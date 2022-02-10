#ifndef MENU_H

#define MENU_H

void initializeStart(bool start[]);

void renderStart1(SDL_Renderer *sdlRenderer, SDL_bool *shallExit, bool start[], TTF_Font *font, char text[]);

void renderStart2(SDL_Renderer *sdlRenderer, Map *map, SDL_bool *shallExit, bool start[], TTF_Font *font, int *number_of_players, int *number_of_blocks, Uint32 regionColor[], Uint32 sarbazkhuneColor[], struct Potion potion[], int *seed);

void renderStart3(SDL_Renderer *sdlRenderer, SDL_bool *shallExit, TTF_Font *font);

void renderStart5(SDL_Renderer *sdlRenderer, Map *map, int number_of_players, int number_of_blocks, SDL_bool *shallExit, struct Potion potion[], char text[], bool start[], TTF_Font *font);

void renderStart6(SDL_Renderer *sdlRenderer, Map *map, int number_of_players, int number_of_blocks, SDL_bool *shallExit, struct Potion potion[], char text[], bool start[], TTF_Font *font);

void readFromTheFile(char champion[], int *highScore);

void writeInFile(char name[], int newScore);

#endif
