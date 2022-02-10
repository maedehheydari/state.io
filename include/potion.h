#ifndef POTION_H

#define POTION_H

void initialize_potions(struct Potion potion[4]);

bool find_the_block_for_potion(Map *map, int number_of_blocks, int x, int y);

void randomPotion( Map *map, int number_of_blocks, SDL_Point *point);

int findPlayersIndex(struct Player *player, Map *map, int number_of_players);

void conflictWithPotion(Map *map, struct Potion potions[4], int number_of_players);

void is_potion_finished(Map *map, int number_of_players);

void initializeImage(SDL_Renderer* sdlRenderer);

void renderPotion(SDL_Renderer *sdlRenderer,  Map *map, int number_of_blocks);

#endif