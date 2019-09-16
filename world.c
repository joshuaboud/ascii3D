#include "world.h"
#include "gameEngine.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define IN_BOUNDS(X,Y) ((X >= 0) && (X < WORLD_SZ) && (Y >= 0) && (Y < WORLD_SZ))

unsigned char world[WORLD_SZ][WORLD_SZ] = {{0}};

void pushCoord(struct CoordQueue *q, int x, int y){
  struct CoordQueue *ptr = (struct CoordQueue *)malloc(sizeof(struct CoordQueue));
  ptr->x = x;
  ptr->y = y;
  ptr->next = q;
  if(!q){
    q = ptr;
  }
}

struct CoordQueue *pullCoord(struct CoordQueue *q){
  struct CoordQueue *ptr = q;
  if(q && q->next)
    q = q->next;
  else
    q = NULL;
  return q;
}

double initWorld(){
  int x = 0, y = 0;
  enum Direction dir = RIGHT;
  double cameraAngle;
  struct CoordQueue *q = (struct CoordQueue *)malloc(sizeof(struct CoordQueue));
  q->next = NULL;
  
  memset((void *)&world, WALL, sizeof(unsigned char)*WORLD_SZ*WORLD_SZ);
  
  srandom(clock());
  
  dir = (random() % 2)? DOWN : RIGHT;
  
  cameraAngle = (dir == DOWN)? 0 : PI/2;
  
  q->x = x;
  q->y = y;
  world[y][x] = EMPTY; // visited
  
  //while(TRUE){
    
  //}
  
  for(int i = 0; i < 100; i++){
    moveXY(dir,&x,&y);
    printf("X: %d Y: %d dir: %d\n",x, y, dir);
    world[y][x] = EMPTY;
    int tempX, tempY;
    do{
      tempX = x;
      tempY = y;
      switch(random()%4){
      case 0:
        dir = (dir + 1)%4;
        break;
      case 1:
        dir = (dir + 3)%4;
        break;
      default:
        break;
      }
      moveXY(dir, &tempX, &tempY);
    }while(!IN_BOUNDS(tempX, tempY));
  }
  
  
  
  /*FILE *fptr = fopen(filename, "r"); // open from file
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
  }*/
  return cameraAngle;
}

void moveXY(int dir, int *x, int *y){
  switch(dir){
  case RIGHT:
    *x += 1;
    break;
  case LEFT:
    *x -= 1;
    break;
  case DOWN:
    *y += 1;
    break;
  case UP:
    *y -= 1;
    break;
  default:
    break;
  }
}
