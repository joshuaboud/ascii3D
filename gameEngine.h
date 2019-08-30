#pragma once

#define WALL 1 // color pair
#define FLOOR 2
#define NUM_SHADES 8 // number of shaded chars
#define FOV 0.01
#define MAX_DIST 16
#define WHT_DST 0
#define PI 3.1415
#define SCR_PLANE 3

typedef struct{
  int width;
  int height;
}ScreenStats;

typedef struct{
  float x;
  float y;
  float a;
}Camera;

int initGame(void);

int gameLoop(void);

void getInput(void);

void drawScreen(void);
