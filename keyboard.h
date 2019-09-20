#pragma once

#include <sys/time.h>
#include <stdio.h>

#define NUM_KEYS 5
extern int keysDown[NUM_KEYS];
enum {keyUP, keyDOWN, keyLEFT, keyRIGHT, keySPACE};

typedef struct{
  struct timeval time;
  unsigned short type;
  unsigned short code;
  unsigned int value;
}input_event;

void proc_kb_event(int fd);
