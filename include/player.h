#ifndef PLAYER_H

#define PLAYER_H

#include "map.h"

typedef struct Block
{

  int x;

  int y;

  int number_of_soldiers;

  struct Player *player;

  struct Block *west;

  struct Block *north_west;

  struct Block *north_east;

  struct Block *east;

  struct Block *south_west;

  struct Block *south_east;

  int status;

  int id;

  void *surface, *texture;

  bool refresh;

  long long time;

   TTF_Font *font;

} Block;

typedef struct RunningPotion
{
  struct Potion* potion;
  long long time;
  bool enabled;
}RunningPotion;

typedef struct Player
{

  Uint32 color;

  Uint32 sarbazkhuneColor;

  bool is_opponent;

  long long time;

  int potionStatus;

  struct RunningPotion potion, otherPotion;

} Player;

typedef struct Soldier
{
  int x;
  int y;
  SDL_Point src;
  SDL_Point dest;

  int srcIndex;
  int destIndex;
  long long time;
  struct Player *player;
  struct Potion *potion;
  bool moved;
  int status;
  bool potionChanged;

} Soldier;

typedef struct PotionNode
{
    SDL_Point point ;
    int potionStatus;
    long long time;
}PotionNode;

long long currentTime();

void move_soldier(Map *map, int srcIndex, int destIndex);

void fix_position(SDL_Point *src, int x, int y, int x2, int y2, int index, int count, bool fromX1);

void initializeImage(SDL_Renderer* sdlRenderer);

void AI( Map *map, int number_of_players, int number_of_blocks);

int is_finished_win(Map *map, int number_of_blocks, int number_of_players);

int is_finished_lose( Map *map, int number_of_blocks, int number_of_players);

int find_the_block( Map *map, int number_of_blocks, int x, int y);

bool find_the_block_for_potion( Map *map, int number_of_blocks, int x, int y);

void randomPotion( Map *map, int number_of_blocks, SDL_Point *point);

void renderPotion(SDL_Renderer *sdlRenderer,  Map *map, int number_of_blocks);

#endif