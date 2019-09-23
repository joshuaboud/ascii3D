#pragma once

#include <sys/time.h>
#include <stdio.h>

#define NUM_KEYS 256
extern int keysDown[NUM_KEYS];
// memory map of key values: 0 - released, 1 - pressed, 2 - held down

typedef struct{
  int fd;
}T_ARGS;

typedef struct{
  struct timeval time;
  unsigned short type;
  unsigned short code;
  unsigned int value;
}input_event;

void init_kb(void);

void *proc_kb_event(void *t_args);
