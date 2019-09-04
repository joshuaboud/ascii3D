#define NCURSES_WIDECHAR 1

#include <ncursesw/ncurses.h>
#include <locale.h>
#include <wchar.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include "gameEngine.h"
#include "world.h"

const cchar_t (gradient[])[NUM_SHADES] = {
  {A_BOLD,0x2588}, // white
  {A_BOLD,0x2593},
  {A_BOLD,0x2592},
  {A_DIM,0x2588},
  {A_DIM,0x2593},
  {A_DIM,0x2592},
  {A_DIM,0x2591},
  {A_DIM,0x20}    // black
};

int wallTexture[TEX_SIZE] = {
  -1,0,0,1,1,0,1,0,0,0,1,1,1,0,0,-1
};

bool running;

pthread_t *tid;
thread_args_t *args;

ScreenStats screen;
Camera camera;
const char *errors[] = {
  "Failed to create thread.",
  "Failed to join thread.",
  "Error opening world file."
};

void error(int err){
  closeGame();
  printf(errors[err]);
  putchar('\n');
  exit(1);
}

int initGame(){
  setlocale(LC_ALL, "");
  initscr(); // initialize ncurses screen
  start_color();
  init_pair(WALL, COLOR_WHITE, COLOR_BLACK);
  init_pair(FLOOR, COLOR_YELLOW, COLOR_BLACK);
  curs_set(0); // turn off cursor display
  nodelay(stdscr,TRUE);
  noecho();
  keypad(stdscr,TRUE);
  cbreak();
  
  camera.x = 1;
  camera.y = 1;
  camera.a = PI/2;
  
  initWorld("level1.dat");
}

void closeGame(){
  nocbreak();
  keypad(stdscr,FALSE);
  echo();
  nodelay(stdscr,FALSE);
  curs_set(1);
  endwin();
}

int gameLoop(){
  
  running = TRUE;
  
  WINDOW *render = newwin(0,0,0,0);
  WINDOW *map = newwin(MAP_SZ,MAP_SZ,0,0);
  
  while(running){
    getInput();
    drawScreen(render,map);
  }
  
  closeGame();
  return 0;
}

void getInput(void){
  switch(getch()){
  case KEY_UP:
    camera.x += cos(camera.a)*0.1;
    if(world[(int)camera.x][(int)camera.y] != 0) camera.x -= cos(camera.a)*0.1;
    camera.y += sin(camera.a)*0.1;
    if(world[(int)camera.x][(int)camera.y] != 0) camera.y -= sin(camera.a)*0.1;
    break;
  case KEY_DOWN:
    camera.x -= cos(camera.a)*0.1;
    if(world[(int)camera.x][(int)camera.y] != 0) camera.x += cos(camera.a)*0.1;
    camera.y -= sin(camera.a)*0.1;
    if(world[(int)camera.x][(int)camera.y] != 0) camera.y += sin(camera.a)*0.1;
    break;
  case KEY_RIGHT:
    camera.a -= 0.05;
    break;
  case KEY_LEFT:
    camera.a += 0.05;
    break;
  default:
    break;
  }
}

void drawScreen(WINDOW *render, WINDOW *map){
  int pthreadFail = 0;
  int lastWidth = screen.width;
  getmaxyx(stdscr,screen.height,screen.width);
  wresize(render,screen.height-MAP_SZ,screen.width);
  mvwin(map,screen.height - MAP_SZ,0);
  if(!tid || screen.width != lastWidth){ // reallocate
    tid = (pthread_t *)malloc(sizeof(pthread_t)*screen.width);
    args = (thread_args_t *)malloc(sizeof(thread_args_t)*screen.width);
  }
  for (int i = 0; i < screen.width; i++){
    args[i].slice = i;
    pthreadFail = pthread_create(&(tid[i]), NULL, threadTrace, (void*)&(args[i]));
    if(pthreadFail) error(THREAD_CREATE);
  }
  for (int i = 0; i < screen.width; i++){
    pthreadFail = pthread_join(tid[i], NULL);
    if(pthreadFail) error(THREAD_JOIN);
    for (int j = 0; j < screen.height-7; j++){
      // print screen one column at a time
      if(fabs(j - screen.height/2) < args[i].wallHeightProjected)
        mvwadd_wch(render,j,i,gradient[args[i].shade]);
      else
        mvwadd_wch(render,j,i,gradient[NUM_SHADES-1]); // floor
    }
    wrefresh(render);
  }
  // draw map
  wmove(map,0,0);
  for (int i = 0; i < MAP_SZ; i++){
    for (int j = 0; j < MAP_SZ; j++){
      if((int)camera.y + (j - MAP_SZ/2) < 0 && (int)camera.x + (i - MAP_SZ/2) < 0){
        mvwaddch(map,j,i,'.');
        continue;
      }
      switch(world[(int)camera.x + (j - MAP_SZ/2)][(int)camera.y + (i - MAP_SZ/2)]){
      case 1:
        mvwaddch(map,j,i,'#');
        break;
      default:
        mvwaddch(map,j,i,'.');
        break;
      }
    }
  }
  mvwaddch(map,MAP_SZ/2,MAP_SZ/2,'@');
  int dirX = MAP_SZ/2 + (int)round(cos(camera.a));
  int dirY = MAP_SZ/2 + (int)round(sin(camera.a));
  mvwaddch(map,dirX,dirY,'-');
  wrefresh(map);
  // draw sprites?
}

void *threadTrace(void *arguments){
  thread_args_t *args = (thread_args_t *)arguments;
  double texDist;
  double angle = camera.a + (screen.width/2 - args->slice)*FOV/screen.width;
  double distance = traceDistance(angle, &texDist);
  int shade = (distance < MAX_DIST)?
  ((int)distance)*NUM_SHADES/MAX_DIST : NUM_SHADES - 1;
  args->wallHeightProjected = (screen.height*WALL_HEIGHT*SCR_PLANE)/(distance);
  args->shade = texture(texDist,shade);
}

double traceDistance(double angle, double *texDist){
  double tracerX = camera.x;
  double tracerY = camera.y;
  while(tracerX < MAX_VIEW && tracerY < MAX_VIEW &&
        tracerX >= 0 && tracerY >=0 &&
        world[(int)tracerX][(int)tracerY] == 0){
    tracerX += cos(angle)*TRACER_INC;
    tracerY += sin(angle)*TRACER_INC;
  }
  *texDist = max((tracerX - floor(tracerX)),(tracerY - floor(tracerY)));
  return sqrt((tracerX - camera.x)*(tracerX - camera.x) + (tracerY - camera.y)*(tracerY - camera.y));
}

double max(double a, double b){
  if(b <= 0.1 || b >= 0.9)
    return a;
  else
    return b;
}

int texture(double texDist, int shade){
  shade -= wallTexture[(int)round(texDist*TEX_SIZE)];
  if(shade < 0) shade = 0;
  if(shade >= NUM_SHADES) shade = NUM_SHADES - 1;
  return shade;
}
