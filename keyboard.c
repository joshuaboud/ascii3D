#include "keyboard.h"
#include "gameEngine.h" // errors
#include <unistd.h>
#include <stdlib.h>
#include <linux/input.h>
#include <fcntl.h>
#include <pthread.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int keysDown[NUM_KEYS] = {0};

pthread_t thread_with_keyboard = 0;

void init_kb(void){
  struct dirent *file = NULL;
  DIR *dp = NULL;
  if((dp = opendir("/dev/input/")) != NULL){
    while((file = readdir(dp))){ // create thread for each file in /dev/input
      const char filepath[256] = "/dev/input/";
      strcat((char *)&filepath, (char *)&file->d_name);
      if(isDirectory(filepath)) continue;
      int kbdev = open(filepath,O_RDONLY | O_NONBLOCK);
      if(kbdev == -1) continue;
      printw("Opening %s\n",filepath);
      refresh();
      T_ARGS *args = (T_ARGS *)malloc(sizeof(T_ARGS));
      args->fd = kbdev;
      pthread_t kbThread;
      if(pthread_create(&kbThread, NULL, proc_kb_event, (void *)args))
        error(THREAD_CREATE);
    }
    closedir(dp);
  }else{
    error(OPEN_DEV_DIR);
  }
  while(thread_with_keyboard == 0);
}

void *proc_kb_event(void *t_args){
  int fd = ((T_ARGS *)t_args)->fd;
  input_event *event = (input_event *)malloc(sizeof(input_event));
  while(!thread_with_keyboard){
    if(read(fd, event, sizeof(input_event)) && event->type == EV_KEY){
      thread_with_keyboard = pthread_self();
      break;
    }
  }
  if(thread_with_keyboard != pthread_self()) pthread_exit(NULL);
  while(running){
    if(read(fd, event, sizeof(input_event)) && event->type == EV_KEY)
      keysDown[event->code] = event->value;
  }
}

int isDirectory(const char *path){
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}
