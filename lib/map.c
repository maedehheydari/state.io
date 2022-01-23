#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <time.h>

#include <math.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL2_gfxPrimitives.h>

#define HEIGHT 800

#define WIDTH 800

#define HEXAGON_LENGTH 40

typedef struct Block {

  int x;

  int y;

  int number_of_soldiers;

  struct Player* player;

  struct Block* west;

  struct Block* north_west;

  struct Block* north_east;

  struct Block* east;

  struct Block* south_west;

  struct Block* south_east;

  int status;

} Block;

typedef struct Player {

  Uint32 color;

  Uint32 sarbazkhuneColor;

  bool is_opponent;

} Player;

typedef struct Map {

  Player** players;

  Block** blocks;

} Map;

bool same_point(int x1, int y1, int x2, int y2) {

  if (((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) <= 100)

    return true;

  else

    return false;

}

void initialize_region_color(Uint32 regionColor[])
{
      regionColor[0] =0x55ffff33 ;
      regionColor[1] = 0x55999999;
      regionColor[2] = 0x55ff00ff;
      regionColor[3] = 0x5500ffff;
      regionColor[4] = 0x55056443;
      regionColor[5] = 0x552240ff;
      regionColor[6] = 0x55403020;
      regionColor[7] = 0x55aa3950;
      regionColor[8] = 0x5587bb45;
      regionColor[9] = 0x55123999;
      regionColor[10] = 0x5540aa30;
      regionColor[11] = 0x55ff6020;
      regionColor[12] = 0x55
      935640;
}

void initialize_sarbazkhune_color(Uint32 sarbazkhuneColor[])
{
      sarbazkhuneColor[0] =0xffffff33 ;
      sarbazkhuneColor[1] = 0xff999999;
      sarbazkhuneColor[2] = 0xffff00ff;
      sarbazkhuneColor[3] = 0xff00ffff;
      sarbazkhuneColor[4] = 0xff056443;
      sarbazkhuneColor[5] = 0xff2240ff;
      sarbazkhuneColor[6] = 0xff403020;
      sarbazkhuneColor[7] = 0xffaa3950;
      sarbazkhuneColor[8] = 0xff87bb45;
      sarbazkhuneColor[9] = 0xff123999;
      sarbazkhuneColor[10] = 0xff40aa30;
      sarbazkhuneColor[11] = 0xffff6020;
      sarbazkhuneColor[12] = 0xff935640;
}

void initialize_player(Map *map, int number_of_players, Uint32 regionColor[], Uint32 sarbazkhuneColor[])
{
   map -> players = (Player**) malloc(number_of_players * sizeof(Player));
   //Uint32 colortest;
  for(int y=0; y<number_of_players; y++)
  {
   // colortest = colortest*(y+y+1);
    //colortest = (colortest^(0xffffff))>>2;
    Player* newplayer = (Player*) malloc(sizeof(Player));
    newplayer -> color = regionColor[y];

        newplayer -> sarbazkhuneColor = sarbazkhuneColor[y];

    map -> players[y] = newplayer;
    if(y == 0 || y == 1)
    {
      map -> players[y] -> is_opponent = false;
    }
    else
    {
      map -> players[y] -> is_opponent = true;
    }
    
  } 
}

void initialize_blocks(Map *map, int number_of_players, int n)// n is a number of blocks here
{
    for(int i=0; i<number_of_players; i++)
    {
      map -> blocks[i]-> status = i;
    }
    for(int i=number_of_players; i<n; i++)
    {
      map -> blocks[i] -> status = 1;
    }
}

void hexagon(SDL_Renderer* sdlRenderer, int x, int y, Uint32 color) {

  boxColor(sdlRenderer, x - sqrt(3) / 2 * HEXAGON_LENGTH - 5, y - HEXAGON_LENGTH / 2 - 5,  x + sqrt(3) / 2 * HEXAGON_LENGTH + 5, y + HEXAGON_LENGTH / 2 + 5,0xffffffff);

  filledTrigonColor(sdlRenderer, x, y - HEXAGON_LENGTH - 5, x - sqrt(3) / 2 * HEXAGON_LENGTH - 5, y - HEXAGON_LENGTH / 2 - 5, x + sqrt(3) / 2 * HEXAGON_LENGTH + 5, y - HEXAGON_LENGTH / 2 - 5, 0xffffffff);

  filledTrigonColor(sdlRenderer, x, y + HEXAGON_LENGTH + 5, x - sqrt(3) / 2 * HEXAGON_LENGTH - 5, y + HEXAGON_LENGTH / 2 + 5, x + sqrt(3) / 2 * HEXAGON_LENGTH + 5, y + HEXAGON_LENGTH / 2 + 5, 0xffffffff);

  boxColor(sdlRenderer, x - sqrt(3) / 2 * HEXAGON_LENGTH, y - HEXAGON_LENGTH / 2 +1,  x + sqrt(3) / 2 * HEXAGON_LENGTH, y + HEXAGON_LENGTH / 2 - 1, color);

  filledTrigonColor(sdlRenderer, x, y - HEXAGON_LENGTH, x - sqrt(3) / 2 * HEXAGON_LENGTH, y - HEXAGON_LENGTH / 2, x + sqrt(3) / 2 * HEXAGON_LENGTH, y - HEXAGON_LENGTH / 2, color);

  filledTrigonColor(sdlRenderer, x, y + HEXAGON_LENGTH, x - sqrt(3) / 2 * HEXAGON_LENGTH, y + HEXAGON_LENGTH / 2, x + sqrt(3) / 2 * HEXAGON_LENGTH, y + HEXAGON_LENGTH / 2, color);

}

void render_map(SDL_Renderer* sdlRenderer, int t, Map* map) {

    Uint32 color = 0x990000ff;

    int n = 0;

    while(n < t) {

      hexagon(sdlRenderer, map -> blocks[n] -> x, map -> blocks[n] -> y, map -> players[map -> blocks[n]-> status] -> color);

      filledCircleColor(sdlRenderer, map -> blocks[n] -> x, map -> blocks[n] -> y, 15,  map -> players[map -> blocks[n]-> status] -> sarbazkhuneColor);
      n++;

    }

}

Map* generate_map(int n) {

  Map* map = (Map*) malloc(sizeof(Map));

  map -> blocks = NULL;

  map -> players = NULL;

  int regions_number = 0;

  Block* block = NULL;

  srand(time(NULL));

  while(regions_number < n) {

    if(block == NULL && regions_number == 0) {

      block = (Block*) malloc(sizeof(Block));

      block -> x = WIDTH / 2;

      block -> y = HEIGHT / 2;

      block -> number_of_soldiers = 0;

      block -> player = NULL;

      block -> east = NULL;

      block -> west = NULL;

      block -> north_east = NULL;

      block -> south_east = NULL;

      block -> north_west = NULL;

      block -> south_west = NULL;

      map -> blocks = (Block**) malloc(n * sizeof(Block));

      map -> blocks[regions_number] = block;

      regions_number ++;

      continue;

    }

    int rnd = rand() % 6 + 1;

    switch(rnd) {

      case(1):

        if(block -> east == NULL && block -> x + sqrt(3) * 3.0 / 2.0 * HEXAGON_LENGTH < WIDTH) {

          Block* newBlock = (Block*) malloc(sizeof(Block));

          newBlock -> x = block -> x + sqrt(3) * HEXAGON_LENGTH;

          newBlock -> y = block -> y;

          newBlock -> number_of_soldiers = 0;

          newBlock -> player = NULL;

          newBlock -> east = NULL;

          newBlock -> west = NULL;

          newBlock -> north_east = NULL;

          newBlock -> south_east = NULL;

          newBlock -> north_west = NULL;

          newBlock -> south_west = NULL;

          for (int i = 0; i < regions_number; i ++) {

            if (same_point(newBlock -> x + sqrt(3) * HEXAGON_LENGTH, newBlock -> y, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> east = map -> blocks[i];

              map -> blocks[i] -> west = newBlock;

            }

            else if (same_point(newBlock -> x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y - HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> north_east = map -> blocks[i];

              map -> blocks[i] -> south_west = newBlock;

            }

            else if (same_point(newBlock -> x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y + HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> south_east = map -> blocks[i];

              map -> blocks[i] -> north_west = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) * HEXAGON_LENGTH, newBlock -> y, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> west = map -> blocks[i];

              map -> blocks[i] -> east = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y - HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> north_west = map -> blocks[i];

              map -> blocks[i] -> south_east = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y + HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> south_west = map -> blocks[i];

              map -> blocks[i] -> north_east = newBlock;

            }

          }

          block = newBlock;

          map -> blocks[regions_number] = block;

          regions_number ++;

        }

        else {

          int rnd_region = rand() % regions_number;

          block = map -> blocks[rnd_region];

        }

        break;

      case(2):

        if(block -> south_east == NULL &&  block -> x + sqrt(3) * HEXAGON_LENGTH < WIDTH && block -> y + HEXAGON_LENGTH * 5.0 / 2.0 < HEIGHT) {

          Block* newBlock = (Block*) malloc(sizeof(Block));

          newBlock -> x = block -> x + sqrt(3) / 2 * HEXAGON_LENGTH;

          newBlock -> y = block -> y + HEXAGON_LENGTH * 3.0 / 2.0;

          newBlock -> number_of_soldiers = 0;

          newBlock -> player = NULL;

          newBlock -> east = NULL;

          newBlock -> west = NULL;

          newBlock -> north_east = NULL;

          newBlock -> south_east = NULL;

          newBlock -> north_west = NULL;

          newBlock -> south_west = NULL;

          for (int i = 0; i < regions_number; i ++) {

            if (same_point(newBlock -> x + sqrt(3) * HEXAGON_LENGTH, newBlock -> y, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> east = map -> blocks[i];

              map -> blocks[i] -> west = newBlock;

            }

            else if (same_point(newBlock -> x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y - HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> north_east = map -> blocks[i];

              map -> blocks[i] -> south_west = newBlock;

            }

            else if (same_point(newBlock -> x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y + HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> south_east = map -> blocks[i];

              map -> blocks[i] -> north_west = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) * HEXAGON_LENGTH, newBlock -> y, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> west = map -> blocks[i];

              map -> blocks[i] -> east = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y - HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> north_west = map -> blocks[i];

              map -> blocks[i] -> south_east = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y + HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> south_west = map -> blocks[i];

              map -> blocks[i] -> north_east = newBlock;

            }

          }

          block = newBlock;

          map -> blocks[regions_number] = block;

          regions_number ++;

        }

        else {

          int rnd_region = rand() % regions_number;

          block = map -> blocks[rnd_region];

        }

        break;

      case(3):

        if(block -> south_west == NULL && block -> x - sqrt(3) * HEXAGON_LENGTH > 0 && block -> y + HEXAGON_LENGTH * 5.0 / 2.0 < HEIGHT) {

          Block* newBlock = (Block*) malloc(sizeof(Block));

          newBlock -> x = block -> x - sqrt(3) / 2 * HEXAGON_LENGTH;

          newBlock -> y = block -> y + HEXAGON_LENGTH * 3.0 / 2.0;

          newBlock -> number_of_soldiers = 0;

          newBlock -> player = NULL;

          newBlock -> east = NULL;

          newBlock -> west = NULL;

          newBlock -> north_east = NULL;

          newBlock -> south_east = NULL;

          newBlock -> north_west = NULL;

          newBlock -> south_west = NULL;

          for (int i = 0; i < regions_number; i ++) {

            if (same_point(newBlock -> x + sqrt(3) * HEXAGON_LENGTH, newBlock -> y, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> east = map -> blocks[i];

              map -> blocks[i] -> west = newBlock;

            }

            else if (same_point(newBlock -> x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y - HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> north_east = map -> blocks[i];

              map -> blocks[i] -> south_west = newBlock;

            }

            else if (same_point(newBlock -> x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y + HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> south_east = map -> blocks[i];

              map -> blocks[i] -> north_west = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) * HEXAGON_LENGTH, newBlock -> y, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> west = map -> blocks[i];

              map -> blocks[i] -> east = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y - HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> north_west = map -> blocks[i];

              map -> blocks[i] -> south_east = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y + HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> south_west = map -> blocks[i];

              map -> blocks[i] -> north_east = newBlock;

            }

          }

          block = newBlock;

          map -> blocks[regions_number] = block;

          regions_number ++;

        }

        else {

          int rnd_region = rand() % regions_number;

          block = map -> blocks[rnd_region];

        }

        break;

      case(4):

        if(block -> west == NULL && block -> x - sqrt(3) * 3.0 / 2.0 * HEXAGON_LENGTH > 0) {

          Block* newBlock = (Block*) malloc(sizeof(Block));

          newBlock -> x = block -> x - sqrt(3) * HEXAGON_LENGTH;

          newBlock -> y = block -> y;

          newBlock -> number_of_soldiers = 0;

          newBlock -> player = NULL;

          newBlock -> east = NULL;

          newBlock -> west = NULL;

          newBlock -> north_east = NULL;

          newBlock -> south_east = NULL;

          newBlock -> north_west = NULL;

          newBlock -> south_west = NULL;

          for (int i = 0; i < regions_number; i ++) {

            if (same_point(newBlock -> x + sqrt(3) * HEXAGON_LENGTH, newBlock -> y, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> east = map -> blocks[i];

              map -> blocks[i] -> west = newBlock;

            }

            else if (same_point(newBlock -> x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y - HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> north_east = map -> blocks[i];

              map -> blocks[i] -> south_west = newBlock;

            }

            else if (same_point(newBlock -> x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y + HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> south_east = map -> blocks[i];

              map -> blocks[i] -> north_west = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) * HEXAGON_LENGTH, newBlock -> y, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> west = map -> blocks[i];

              map -> blocks[i] -> east = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y - HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> north_west = map -> blocks[i];

              map -> blocks[i] -> south_east = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y + HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> south_west = map -> blocks[i];

              map -> blocks[i] -> north_east = newBlock;

            }

          }

          block = newBlock;

          map -> blocks[regions_number] = block;

          regions_number ++;

        }

        else  {

          int rnd_region = rand() % regions_number;

          block = map -> blocks[rnd_region];

        }

        break;

      case(5):

        if(block -> north_west == NULL && block -> x - sqrt(3) * HEXAGON_LENGTH > 0 && block -> y - HEXAGON_LENGTH * 5.0 / 2.0 > 0) {

          Block* newBlock = (Block*) malloc(sizeof(Block));

          newBlock -> x = block -> x - sqrt(3) / 2 * HEXAGON_LENGTH;

          newBlock -> y = block -> y - HEXAGON_LENGTH * 3.0 / 2.0;

          newBlock -> number_of_soldiers = 0;

          newBlock -> player = NULL;

          newBlock -> east = NULL;

          newBlock -> west = NULL;

          newBlock -> north_east = NULL;

          newBlock -> south_east = NULL;

          newBlock -> north_west = NULL;

          newBlock -> south_west = NULL;

          for (int i = 0; i < regions_number; i ++) {

            if (same_point(newBlock -> x + sqrt(3) * HEXAGON_LENGTH, newBlock -> y, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> east = map -> blocks[i];

              map -> blocks[i] -> west = newBlock;

            }

            else if (same_point(newBlock -> x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y - HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> north_east = map -> blocks[i];

              map -> blocks[i] -> south_west = newBlock;

            }

            else if (same_point(newBlock -> x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y + HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> south_east = map -> blocks[i];

              map -> blocks[i] -> north_west = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) * HEXAGON_LENGTH, newBlock -> y, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> west = map -> blocks[i];

              map -> blocks[i] -> east = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y - HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> north_west = map -> blocks[i];

              map -> blocks[i] -> south_east = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y + HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> south_west = map -> blocks[i];

              map -> blocks[i] -> north_east = newBlock;

            }

          }

          block = newBlock;

          map -> blocks[regions_number] = block;

          regions_number ++;

        }

        else {

          int rnd_region = rand() % regions_number;

          block = map -> blocks[rnd_region];

        }

        break;

      case(6):

        if(block -> north_east == NULL && block -> x + sqrt(3) * HEXAGON_LENGTH < WIDTH && block -> y - HEXAGON_LENGTH * 5.0 / 2.0 > 0) {

          Block* newBlock = (Block*) malloc(sizeof(Block));

          newBlock -> x = block -> x + sqrt(3) / 2 * HEXAGON_LENGTH;

          newBlock -> y = block -> y - HEXAGON_LENGTH * 3.0 / 2.0;

          newBlock -> number_of_soldiers = 0;

          newBlock -> player = NULL;

          newBlock -> east = NULL;

          newBlock -> west = NULL;

          newBlock -> north_east = NULL;

          newBlock -> south_east = NULL;

          newBlock -> north_west = NULL;

          newBlock -> south_west = NULL;

          for (int i = 0; i < regions_number; i ++) {

            if (same_point(newBlock -> x + sqrt(3) * HEXAGON_LENGTH, newBlock -> y, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> east = map -> blocks[i];

              map -> blocks[i] -> west = newBlock;

            }

            else if (same_point(newBlock -> x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y - HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> north_east = map -> blocks[i];

              map -> blocks[i] -> south_west = newBlock;

            }

            else if (same_point(newBlock -> x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y + HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> south_east = map -> blocks[i];

              map -> blocks[i] -> north_west = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) * HEXAGON_LENGTH, newBlock -> y, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> west = map -> blocks[i];

              map -> blocks[i] -> east = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y - HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> north_west = map -> blocks[i];

              map -> blocks[i] -> south_east = newBlock;

            }

            else if (same_point(newBlock -> x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock -> y + HEXAGON_LENGTH * 3.0 / 2.0, map -> blocks[i] -> x, map -> blocks[i] -> y)) {

              newBlock -> south_west = map -> blocks[i];

              map -> blocks[i] -> north_east = newBlock;

            }

          }

          block = newBlock;

          map -> blocks[regions_number] = block;

          regions_number ++;

        }

        else {

          int rnd_region = rand() % regions_number;

          block = map -> blocks[rnd_region];

        }

        break;

      }

    }

    return map;

};
