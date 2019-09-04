#include "world.h"
#include "gameEngine.h"
#include <stdio.h>

unsigned char world[WORLD_SZ][WORLD_SZ] = {{0}};

int initWorld(char *filename){
  FILE *fptr = fopen(filename, "r");
  if(!fptr) error(OPEN_LVL);
  
  char c;
  int i = 0, j = 0;
  
  while((c = fgetc(fptr)) != EOF && i < WORLD_SZ && j < WORLD_SZ){
    switch(c){
    case '#':
      world[i][j++] = 1;
      break;
    case '\n':
      j = 0;
      i++;
      break;
    default:
      j++;
      break;
    }
  }
  
  /*for (int i = 0; i < 64; i++){
    // draw perimiter
    world[0][i] = 1;
    world[64-1][i] = 1;
    world[i][64-1] = 1;
    world[i][0] = 1;
  }
  world[32][32] = 1;
  world[32][33] = 1;
  world[32][34] = 1;
  world[33][34] = 1;
  world[34][34] = 1;
  world[34][33] = 1;
  world[34][32] = 1;
  world[33][32] = 1;*/
  return 0;
}
