#define NCURSES_WIDECHAR 1

#include <ncursesw/ncurses.h>
#include <locale.h>
#include <wchar.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
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

bool running;

ScreenStats screen;
Camera camera;

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

int gameLoop(){
  
  running = TRUE;
  
  while(running){
    getInput();
    drawScreen();
  }
  
  endwin();
  return 0;
}

void getInput(){
  switch(getch()){
  case KEY_UP:
    camera.x += cos(camera.a)*0.1;
    camera.y += sin(camera.a)*0.1;
    break;
  case KEY_DOWN:
    camera.x -= cos(camera.a)*0.1;
    camera.y -= sin(camera.a)*0.1;
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

void drawScreen(){
  getmaxyx(stdscr,screen.height,screen.width);
  // draw walls
  for (int i = 0; i < screen.width; i++){
    double angle = camera.a + (screen.width/2 - i) * FOV;
    double tracerX = camera.x;
    double tracerY = camera.y;
    while(tracerX < WORLD_SZ && tracerY < WORLD_SZ &&
          tracerX >= 0 && tracerY >=0 &&
          world[(int)tracerX][(int)tracerY] == 0){
      tracerX += cos(angle)*0.1;
      tracerY += sin(angle)*0.1;
    }
    float distance = sqrt((tracerX - camera.x)*(tracerX - camera.x) +
                          (tracerY - camera.y)*(tracerY - camera.y));
    int shade = ((int)distance - WHT_DST)*NUM_SHADES/MAX_DIST;
    int wallMask = (MAX_DIST - distance)*screen.height/(MAX_DIST)*0.5;
    for (int j = 0; j < screen.height; j++){
      // print screen one column at a time
      if(fabs(j - screen.height/2) < wallMask)
        mvadd_wch(j,i,gradient[shade]);
      else
        mvadd_wch(j,i,gradient[NUM_SHADES-1]); // floor
    }
  }
  // draw map
  move(0,0);
  for (int i = 0; i < WORLD_SZ; i++){
    for (int j = 0; j < WORLD_SZ; j++){
      switch(world[j][i]){
      case 1:
        mvaddch(j,i,'#');
        break;
      default:
        break;
      }
    }
  }
  mvaddch((int)camera.x,(int)camera.y,'@');
  int dirX = camera.x + (int)round(cos(camera.a));
  int dirY = camera.y + (int)round(sin(camera.a));
  mvaddch(dirX,dirY,'-');
  // draw sprites?
  refresh();
}
