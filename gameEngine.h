#pragma once

#define WALL 1 // color pair
#define FLOOR 2
#define NUM_SHADES 8 // number of shaded chars
#define FOV (PI/2)
#define MAX_DIST 15
#define MAX_VIEW 64
#define PI 3.1415
#define SCR_PLANE 7.5
#define TRACER_INC 0.01
#define WALL_HEIGHT 0.1
#define TEX_SIZE 16
#define MAX_KEYS 10

typedef struct{
  int width;
  int height;
}ScreenStats;

typedef struct{
  double x;
  double y;
  double a;
}Camera;

int initGame(void);

int gameLoop(void);

void getInput(void);

void drawScreen(void);

double traceDistance(double angle, double *texDist);

double max(double a, double b);

int texture(double texDist, int shade);
