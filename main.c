#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <math.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "include/map.h"

#define HEIGHT 800

#define WIDTH 800

#define HEXAGON_LENGTH 40


int main() {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {

    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

    return 0;

  }

  SDL_Window* sdlWindow = SDL_CreateWindow("Test_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

  SDL_Renderer* sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

  const int FPS = 60;

  SDL_bool shallExit = SDL_FALSE;

  int number_of_players = 6;

  int number_of_blocks = 30;

  Map* map = generate_map(number_of_blocks);

  Uint32 regionColor[12];

  initialize_region_color(regionColor);

    Uint32 sarbazkhuneColor[12];

  initialize_sarbazkhune_color(sarbazkhuneColor);

  initialize_player(map, number_of_players, regionColor, sarbazkhuneColor);

  initialize_blocks(map, number_of_players, number_of_blocks);


  while (shallExit == SDL_FALSE) {

    SDL_Event sdlEvent;

    render_map(sdlRenderer, number_of_blocks, map);

    SDL_RenderPresent(sdlRenderer);

    SDL_Delay(1000 / FPS);

    while (SDL_PollEvent(&sdlEvent)) {

      switch (sdlEvent.type) {

        case SDL_QUIT:

          shallExit = SDL_TRUE;

          break;

      }

    }

  }

  SDL_DestroyWindow(sdlWindow);

  SDL_Quit();

  return 0;

}
