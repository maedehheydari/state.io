#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <time.h>

#include <math.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL2_gfxPrimitives.h>



// typedef struct Player {

//   Uint32 color;

//   Uint32 sarbazkhuneColor;

//   bool is_opponent;

//   struct Grohan* grohan;

//   int number_of_grohans;

// } Player;

// typedef struct Button
// {
//   int x;
//   int y;
// }Button;

// typedef struct Soldier
// {
//     int x;
//     int y;
//     int power;
//     int status;
//     int number_of_block;
// } Soldier;

// typedef struct Grohan
// {
//     int x;
//     int y;
//     int number_of_soldiers;
//     Soldier* soldier;
//      Button* source;
//     Button* destination;
// } Grohan;

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

  int id;

} Block;

typedef struct Player {

  Uint32 color;

  Uint32 sarbazkhuneColor;

  bool is_opponent;

} Player;

typedef struct Maajon
{
  int pow;
  void (*movement)();
  void (*power)();

}Maajon;

typedef struct Soldier
{
  int x;
  int y;
  SDL_Point src;
  SDL_Point dest;
  struct Player * player;
  struct Maajon* maajon;

}Soldier;













