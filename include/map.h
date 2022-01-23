#ifndef MAP_H

#define MAP_H

struct Block {

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


};

typedef struct Block Block;

struct Player {

  Uint32 color;

   Uint32 sarbazkhuneColor;

  bool is_opponent;

};

typedef struct Player Player;

struct Map {

  Player** players;

  Block** blocks;

};

typedef struct Map Map;

Map* generate_map(int n);

void initialize_region_color(Uint32 regionColor[]);

void initialize_player(Map *map, int number_of_players, Uint32 regionColor[], Uint32 sarbazkhuneColor[]);

void render_map(SDL_Renderer* sdlRenderer, int t, Map* map);

void hexagon(SDL_Renderer* sdlRenderer, int x, int y, Uint32 color);

void initialize_blocks(Map *map, int number_of_players, int n);// n is a number of blocks here

void initialize_sarbazkhune_color(Uint32 sarbazkhuneColor[]);


#endif
