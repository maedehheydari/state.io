#ifndef MAP_H

#define MAP_H






struct Map {

  struct Player** players;

  struct Block** blocks;

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
