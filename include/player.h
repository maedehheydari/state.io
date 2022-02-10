#ifndef PLAYER_H

#define PLAYER_H

#include "map.h"

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

int find_the_block(Map *map, int number_of_blocks, int x, int y);

void generate_soldier(Map *map, int number_of_blocks);

bool usable_function(SDL_Point *point, Map *map, struct LinkedListNode *soldier_node, long long time, bool render);

void move_soldier(Map *map, int srcIndex, int destIndex);

void merge_soldiers(Map *map);

void render_soldier(SDL_Renderer *SDL_Renderer, Map *map);

void AI(Map *map, int number_of_players, int number_of_blocks);

int is_finished_win( Map *map, int number_of_blocks, int number_of_players);

int is_finished_lose( Map *map, int number_of_blocks, int number_of_players);

#endif