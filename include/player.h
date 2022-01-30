#ifndef PLAYER_H

#define PLAYER_H


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




#endif