#include "world.h"
#include "gameEngine.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define IN_BOUNDS(X,Y) ((X >= 0) && (X < WORLD_SZ) && (Y >= 0) && (Y < WORLD_SZ))

unsigned char world[WORLD_SZ][WORLD_SZ] = {{0}};

void initWorld(){
  int x = 0, y = 0;
  
  memset((void *)&world, WALL, sizeof(unsigned char)*WORLD_SZ*WORLD_SZ);
  
  srandom(clock());
  
  recursiveGen(x,y);
}

void recursiveGen(int x, int y){
  while(!allVisited(x,y)){
    int oldX = x, oldY = y;
    while(TRUE){
      switch(random()%4){
      case 0:
        x += 2;
        break;
      case 1:
        y += 2;
        break;
      case 2:
        x -= 2;
        break;
      case 3:
        y -= 2;
        break;
      }
      if(!IN_BOUNDS(x,y) || world[y][x] == EMPTY){
        x = oldX;
        y = oldY;
      }else{
        break;
      }
    }
    world[(y+oldY)/2][(x+oldX)/2] = EMPTY;
    world[y][x] = EMPTY;
    recursiveGen(x,y);
  }
}

int allVisited(int x, int y){
  return (!IN_BOUNDS(x-2,y) || world[y][x-2] == EMPTY) &&
         (!IN_BOUNDS(x+2,y) || world[y][x+2] == EMPTY) &&
         (!IN_BOUNDS(x,y-2) || world[y-2][x] == EMPTY) &&
         (!IN_BOUNDS(x,y+2) || world[y+2][x] == EMPTY);
}
