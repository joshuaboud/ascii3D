#include "keyboard.h"
#include "gameEngine.h" // errors
#include <unistd.h>
#include <stdlib.h>
#include <linux/input.h>
#include <fcntl.h>
#include <pthread.h>

int keysDown[NUM_KEYS] = {0};

void init_kb(void){
  int kbdev = open("/dev/input/event0",O_RDONLY | O_NONBLOCK);
  T_ARGS args = {kbdev};
  pthread_t kbThread;
  if(pthread_create(&kbThread, NULL, proc_kb_event, (void *)&args))
    error(THREAD_CREATE);
}

void *proc_kb_event(void *t_args){
  int fd = ((T_ARGS *)t_args)->fd;
  input_event *event = (input_event *)malloc(sizeof(input_event));
  while(running){
    if(read(fd, event, sizeof(input_event)) && event->type == EV_KEY)
      keysDown[event->code] = event->value;
  }
}


