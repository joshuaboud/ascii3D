#include "keyboard.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <fcntl.h>

int keysDown[NUM_KEYS] = {0};

void proc_kb_event(int fd){
  input_event *event = (input_event *)malloc(sizeof(input_event));
  if(!read(fd, event, sizeof(input_event)) || event->type != EV_KEY) return;
  
  switch(event->code){
    case KEY_UP:
      keysDown[keyUP] = event->value;
      break;
    case KEY_DOWN:
      keysDown[keyDOWN] = event->value;
      break;
    case KEY_LEFT:
      keysDown[keyLEFT] = event->value;
      break;
    case KEY_RIGHT:
      keysDown[keyRIGHT] = event->value;
      break;
    case KEY_SPACE:
      keysDown[keySPACE] = event->value;
      break;
    default:
      break;
  }
}


