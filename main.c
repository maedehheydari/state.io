#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <math.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL2_gfxPrimitives.h>

#include <time.h>

#include "include/map.h"

#include "include/player.h"

#define HEIGHT 800

#define WIDTH 800

#define HEXAGON_LENGTH 70

int find_the_block(Map *map,int number_of_blocks, int x, int y)
{
  for(int i=0; i<number_of_blocks; i++)
  {
    // if( (x < map->blocks[i]->x + sqrt(3)/2.0 * l && x > map->blocks[i]->x - sqrt(3)/2.0 * l && y <  map->blocks[i]->y+l/2.0 && y >  map->blocks[i]->y-l/2.0) 
    // || ())
    //      filledCircleColor(sdlRenderer, map -> blocks[n] -> x, map -> blocks[n] -> y, 15,  map -> players[map -> blocks[n]-> status] -> sarbazkhuneColor);
    if((x < map->blocks[i]->x + 15 && x > map->blocks[i]->x -15) && (y <  map->blocks[i]->y +15 && y >  map->blocks[i]->y -15))
    {
      return i;
    }

  }
  return -1;
}



/*void initialize_x_y_soldiers(Map *map, SDL_Renderer sdlRenderer, int number_of_players)
{
  int r = 5;
  int distance = 3;
  for(int i=0; i<number_of_players; i++)
  {
    for(int j=0; j< map->players[i]->number_of_grohans; j++ )
    {
      int col = map->players[i]->grohan[j]->number_of_soldiers / row;
      int t = 3;
      for(int k=0; k<col; k++)
      {
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->x = map->players[i]->grohan[j]->source->x + (map->players[i]->grohan[j]->destination->x - map->players[i]->grohan[j]->source->x) * t;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->y = map->players[i]->grohan[j]->source->y + (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * t;
       
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-1]->x = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->x - (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * distance;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-1]->y = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->y - (map->players[i]->grohan[j]->source->x - map->players[i]->grohan[j]->destination->x) * distance;

        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-2]->x = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->x - (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * distance*2;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-2]->y = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->y - (map->players[i]->grohan[j]->source->x - map->players[i]->grohan[j]->destination->x) * distance*2;

        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1+1]->x = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->x + (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * distance;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1+1]->y = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->y + (map->players[i]->grohan[j]->source->x - map->players[i]->grohan[j]->destination->x) * distance;

        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1+2]->x = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->x + (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * distance*2;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1+2]->y = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->y + (map->players[i]->grohan[j]->source->x - map->players[i]->grohan[j]->destination->x) * distance*2;

        t -= 3;

      }
      int mod = map->players[i]->grohan[j]->number_of_soldiers % row;
      int temp_x = map->players[i]->grohan[j]->source->x + (map->players[i]->grohan[j]->destination->x - map->players[i]->grohan[j]->source->x) * t;
      int temp_y = map->players[i]->grohan[j]->source->y + (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * t;
      if(mod > 0)
      {
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-2]->x = temp_x - (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * distance*2;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-2]->y = temp_y - (map->players[i]->grohan[j]->source->x - map->players[i]->grohan[j]->destination->x) * distance*2;
      }
      if(mod>1)
      {
         map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-1]->x = temp_x - (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * distance;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-1]->y = temp_y - (map->players[i]->grohan[j]->source->x - map->players[i]->grohan[j]->destination->x) * distance;

      }
      if(mod>2)
      {
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->x = temp_x
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->y = temp_y;
      }
      if(mod>3)
      {
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1+1]->x = temp_x + (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * distance;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1+1]->y = temp_y + (map->players[i]->grohan[j]->source->x - map->players[i]->grohan[j]->destination->x) * distance;

      }
    }
  }
}*/


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

  /*initialize_x_y_soldiers(map, sdlRenderer, number_of_players);*/

  /*Button source;

   Button destination;*/


  bool touch;
  int srcIndex = -1, destIndex = -1;

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

          /*case SDL_MOUSEBUTTONDOWN:
            if(source_is_empty)
              {
                SDL_GetMouseState(&source.x, &source.y);
                source_is_empty = false;
              }
            else
              {
                SDL_GetMouseState(&destination.x, &destination.y);
                source_is_empty = true;
              }

            break;*/

          
            case SDL_MOUSEBUTTONDOWN: 
              if (srcIndex == -1) 
              {
                  srcIndex = find_the_block(map, number_of_blocks, sdlEvent.button.x, sdlEvent.button.y);
        
                   touch =  srcIndex != -1;

              } 
              else 
              {
                 destIndex = find_the_block(map, number_of_blocks, sdlEvent.button.x, sdlEvent.button.y);

                 if(srcIndex == destIndex)
                 {
                   srcIndex = destIndex = -1;
                 }
                 touch = destIndex != -1;
              }


              case SDL_MOUSEBUTTONUP:
                if(touch)
                {
                  if(destIndex == -1)
                  {
                    int a = find_the_block(map, number_of_blocks, sdlEvent.button.x, sdlEvent.button.y);
                    if(a != srcIndex)
                    {
                      srcIndex = -1;
                    }
                  }
                  else
                  {
                    int a = find_the_block(map, number_of_blocks, sdlEvent.button.x, sdlEvent.button.y);
                    if(a != destIndex)
                    {
                      destIndex = -1;
                    }
                    else if(a != -1)
                    {
                      printf("%d %d\n", a, srcIndex);
                    }
                    srcIndex = destIndex = -1;
                  }
                }
                touch = false;
              






      }

    }

  }

  SDL_DestroyWindow(sdlWindow);

  SDL_Quit();

  return 0;

}
