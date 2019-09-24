#pragma once

#define WORLD_SZ 256
#define PI 3.1415
#define EMPTY 0
#define WALL 1

extern unsigned char world[WORLD_SZ][WORLD_SZ];

void initWorld(void);
// generates maze in world[][], returns initial camera angle

void recursiveGen(int x, int y);

int allVisited(int x, int y);
// returns true if all adjacent cells of (x,y) are visited
