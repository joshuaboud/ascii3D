#pragma once

#include <ncursesw/ncurses.h>

#define WALL 1 // color pair
#define FLOOR 2
#define NUM_SHADES 8 // number of shaded chars
#define FOV (PI/2)
#define MAX_DIST 15
#define MAX_VIEW 64
#define PI 3.1415
#define SCR_PLANE 10
#define TRACER_INC 0.01
#define WALL_HEIGHT 0.1
#define TEX_SIZE 16
#define MAX_KEYS 10
#define HUD_SZ 7
#define WALK_SPEED 10
#define TURN_SPEED 2*PI
#define INPUT_DELAY 5000000

enum {THREAD_CREATE, THREAD_JOIN, OPEN_LVL};

typedef struct{
  int width;
  int height;
}ScreenStats;

typedef struct{
  double x;
  double y;
  double a;
}Camera;

typedef struct{
  int slice;
  int wallHeightProjected;
  int shade;
}thread_args_t;

void error(int err);

int initGame(void);

void closeGame(void);

int gameLoop(void);

void *threadInput(void *args);

void drawScreen(WINDOW *render, WINDOW *map);

void *threadTrace(void *arguments);

double traceDistance(double angle, double *texDist);

double max(double a, double b);

int texture(double texDist, int shade);
