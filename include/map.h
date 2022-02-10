#ifndef MAP_H

#define MAP_H

typedef struct Map
{
  struct Player **players;
  struct Block **blocks;
  struct LinkedListNode *soldiers;
  struct LinkedListNode *potions;
} Map;

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

typedef struct Potion
{
  /*
  1->myspeed blue
  2->others speed green 
  3->defended  red
  4->rate of generation purple
  */
  Uint32 color;
  int status;
  int rate_of_generation;
  int duration;
} Potion;

typedef struct Rect
{
  int x1;
  int y1;
  int x2;
  int y2;
} Rect;

bool same_point(int x1, int y1, int x2, int y2);

bool is_in_button(int x, int y, Rect rect);

void initialize_region_color(Uint32 regionColor[]);

void initialize_sarbazkhune_color(Uint32 sarbazkhuneColor[]);

void initialize_player(Map *map, int number_of_players, Uint32 regionColor[], Uint32 sarbazkhuneColor[]);

void initialize_blocks(Map *map, int number_of_players, int n, int seed); // n is a number of blocks here

void free_surface(Block *o);

void render_label(SDL_Renderer *renderer, Block *block, int x, int y);

void renderLabelMenu(SDL_Renderer *sdlRenderer, Rect rect, char label[], TTF_Font *font);

void renderLabelMenuTextbox(SDL_Renderer *sdlRenderer, Rect rect, char label[], TTF_Font *font, int len);

void renderLabelStart3(SDL_Renderer *sdlRenderer, char label[], TTF_Font *font, int number);

void hexagon(SDL_Renderer *sdlRenderer, int x, int y, Uint32 color);

void ellipse(SDL_Renderer *sdlRenderer, int x, int y, Uint32 color);

void potionizedSarbazkhune(SDL_Renderer *sdlRenderer, int x, int y, Uint32 color);

void render_map(SDL_Renderer *sdlRenderer, int t, Map *map);

void initializeStart(bool start[]);

void generateStart4(SDL_Renderer *sdlRenderer, Map **map, int number_of_players, int number_of_blocks, Uint32 regionColor[], Uint32 sarbazkhuneColor[], struct Potion potion[], int seed);

void renderStart1(SDL_Renderer *sdlRenderer, SDL_bool *shallExit, bool start[], TTF_Font *font, char text[]);

void renderStart2(SDL_Renderer *sdlRenderer, Map *map, SDL_bool *shallExit, bool start[], TTF_Font *font, int *number_of_players, int *number_of_blocks, Uint32 regionColor[], Uint32 sarbazkhuneColor[], struct Potion potion[], int *seed);

void renderStart3(SDL_Renderer *sdlRenderer, SDL_bool *shallExit, TTF_Font *font);

void renderStart4(SDL_Renderer *sdlRenderer, Map *map, int number_of_players, int number_of_blocks, SDL_bool *shallExit, struct Potion potion[], char text[], bool start[]);

void renderStart5(SDL_Renderer *sdlRenderer, Map *map, int number_of_players, int number_of_blocks, SDL_bool *shallExit, struct Potion potion[], char text[], bool start[], TTF_Font *font);

void renderStart6(SDL_Renderer *sdlRenderer, Map *map, int number_of_players, int number_of_blocks, SDL_bool *shallExit, struct Potion potion[], char text[], bool start[], TTF_Font *font);

void readFromTheFile(char champion[], int *highScore);

void writeInFile(char name[], int newScore);

Map *generate_map(int n, int seed);

#endif