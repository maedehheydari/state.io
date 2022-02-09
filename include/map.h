#ifndef MAP_H

#define MAP_H

struct Map
{

  struct Player **players;

  struct Block **blocks;

  struct LinkedListNode *soldiers;

    struct LinkedListNode *potions;

};

typedef struct Map Map;

typedef struct Potion
{
  /*
  1->myspeed  dark blue
  2->others speed   green
  3->defended   pink
  4->rate of generation   light blue
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

Map *generate_map(int n, int seed);

void initialize_region_color(Uint32 regionColor[]);

void initialize_player(Map *map, int number_of_players, Uint32 regionColor[], Uint32 sarbazkhuneColor[]);

void initialize_potions(struct Potion potion[4]);

void render_map(SDL_Renderer *sdlRenderer, int t, Map *map);

void hexagon(SDL_Renderer *sdlRenderer, int x, int y, Uint32 color);

void ellipse(SDL_Renderer* sdlRenderer,int x, int y, Uint32 color);

void initialize_blocks(Map *map, int number_of_players, int n, int seed); // n is a number of blocks here

void initialize_sarbazkhune_color(Uint32 sarbazkhuneColor[]);

  void conflictWithPotion(Map * map, struct Potion potion[4], int number_of_players);

void is_potion_finished(Map *map, int number_of_players);

void render_soldier(SDL_Renderer *SDL_Renderer, Map *map);

void generateStart4(SDL_Renderer* sdlRenderer,Map **map, int number_of_players, int number_of_blocks, Uint32 regionColor[], Uint32 sarbazkhuneColor[], struct Potion potion[], int seed);

void initializeStart(bool start[]);

void renderStart4(SDL_Renderer *sdlRenderer, Map *map, int number_of_players, int number_of_blocks, SDL_bool *shallExit, struct Potion potion[], char text[]);

bool is_in_button(int x, int y, Rect rect);

void readFromTheFile(char champion[], int *highScore);

//int renderStart1test(SDL_Renderer *sdlRenderer, SDL_bool *shallExit,struct TTF_Font *font);

void renderStart1(SDL_Renderer *sdlRenderer, SDL_bool *shallExit, bool start[],   TTF_Font * font, char text[]);

void renderLabelMenu(SDL_Renderer *sdlRenderer, Rect rect, char label[],  TTF_Font *font);

void renderLabelMenuTextbox(SDL_Renderer *sdlRenderer, Rect rect, char label[], TTF_Font *font,int  len);

void renderStart2(SDL_Renderer *sdlRenderer, Map* map, SDL_bool *shallExit, bool start[],  TTF_Font *font, int* number_of_players, int* number_of_blocks, Uint32 regionColor[], Uint32 sarbazkhuneColor[], struct Potion potion[], int* seed);

void renderStart3(SDL_Renderer *sdlRenderer, SDL_bool *shallExit, TTF_Font* font);

void writeInFile(char name[], int newScore);

void generate_soldier(Map *map, int number_of_blocks);

#endif