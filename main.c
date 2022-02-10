#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "include/map.h"
#include "include/player.h"
#include "include/linkedlist.h"
#include "include/menu.h"
#include "include/potion.h"

#define HEIGHT 800
#define WIDTH 800
#define HEXAGON_LENGTH 70
#define R_SARBAZKHUNEH 15
#define R_POTION 20
#define R_SOLDIER 5

int main()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
  TTF_Init();
  IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
  SDL_Window *sdlWindow = SDL_CreateWindow("Test_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  const int FPS = 60;
  int number_of_players = 7;
  int number_of_blocks = 30;
  Map *map;
  Uint32 regionColor[12];
  Uint32 sarbazkhuneColor[12];
  int seed = 0;
  struct Potion potion[4];
  SDL_bool shallExit = SDL_FALSE;
  bool start[5];
  bool gen = true;
  initializeStart(start);
  TTF_Font *font = TTF_OpenFont("padmaa.ttf", 22);
  char text[50];
  while (shallExit == SDL_FALSE)
  {
    if (start[0])
      renderStart1(sdlRenderer, &shallExit, start, font, text);
    else if (start[1])
      renderStart2(sdlRenderer, map, &shallExit, start, font, &number_of_players, &number_of_blocks, regionColor, sarbazkhuneColor, potion, &seed);
    else if (start[2])
      renderStart3(sdlRenderer, &shallExit, font);
    else if (gen && start[3])
    {
      generateStart4(sdlRenderer, &map, number_of_players, number_of_blocks, regionColor, sarbazkhuneColor, potion, seed);
      gen = false;
      renderStart4(sdlRenderer, map, number_of_players, number_of_blocks, &shallExit, potion, text, start);
    }
    else if (start[3])
      renderStart4(sdlRenderer, map, number_of_players, number_of_blocks, &shallExit, potion, text, start);
    else if (start[4])
      renderStart5(sdlRenderer, map, number_of_players, number_of_blocks, &shallExit, potion, text, start, font);
    else if (start[5])
      renderStart6(sdlRenderer, map, number_of_players, number_of_blocks, &shallExit, potion, text, start, font);
  }
  SDL_DestroyRenderer(sdlRenderer);
  SDL_DestroyWindow(sdlWindow);
  SDL_Quit();
  return 0;
}