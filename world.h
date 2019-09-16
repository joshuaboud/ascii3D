#pragma once

#define WORLD_SZ 256
#define PI 3.1415
#define EMPTY 0
#define WALL 1

enum Direction{RIGHT, LEFT, DOWN, UP};

struct CoordQueue{
  int x;
  int y;
  struct CoordQueue *next;
};

extern unsigned char world[WORLD_SZ][WORLD_SZ];

void pushCoord(struct CoordQueue *q, int x, int y);

struct CoordQueue *pullCoord(struct CoordQueue *q);

double initWorld(void);
// generates maze in world[][], returns initial camera angle

void moveXY(int dir, int *x, int *y);
