#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include "../include/linkedlist.h"
#include <SDL2/SDL_ttf.h>
#include "../include/player.h"
#include "../include/map.h"
#include "../include/menu.h"
#include "../include/potion.h"

#define HEIGHT 800
#define WIDTH 800
#define HEXAGON_LENGTH 70
#define R_SOLDIER 5
#define R_POTION 20
#define is_in_area(x, y, x1, x2, y1, y2) ((x) >= (x1) && (x) <= (x2) && (y) >= (y1) && (y) <= (y2))
#define LIMITATION_OF_SOLDIERS(playerStatus) (playerStatus == 1 ? 0 : 50)

void initialize_potions(struct Potion potion[4])
{
  for (int i = 0; i < 4; i++)
  {
    potion[i].status = i + 1;
    potion[i].rate_of_generation = 1;
    potion[i].duration = 10000;
  }
  potion[3].rate_of_generation = 2;
  potion[0].color = 0xffbf6b17;
  potion[1].color = 0xff999900;
  potion[2].color = 0xff17179b;
  potion[3].color = 0xffaa3b71;
}

bool find_the_block_for_potion(Map *map, int number_of_blocks, int x, int y)
{
  for (int i = 0; i < number_of_blocks; i++)
  {
    if ((x < map->blocks[i]->x + HEXAGON_LENGTH * 1.0 * sqrt(3) / 2 && x > map->blocks[i]->x - HEXAGON_LENGTH * 1.0 * sqrt(3) / 2) && (y < map->blocks[i]->y + HEXAGON_LENGTH * 1.0 * sqrt(3) / 2 && y > map->blocks[i]->y - HEXAGON_LENGTH * 1.0 * sqrt(3) / 2))
      return true;
  }
  return false;
}

void randomPotion( Map *map, int number_of_blocks, SDL_Point *point)
{
  srand(time(NULL));
  point->x = rand() % WIDTH;
  point->y = rand() % HEIGHT;
  while (!find_the_block_for_potion(map, number_of_blocks, point->x, point->y) && find_the_block(map, number_of_blocks, point->x, point->y) == -1)
  {
    point->x = rand() % WIDTH;
    point->y = rand() % HEIGHT;
  }
}

int findPlayersIndex(struct Player *player, Map *map, int number_of_players)
{
  for (int i = 0; i < number_of_players; i++)
  {
    if (map->players[i] == player)
      return i;
  }
  return 1;
}

void conflictWithPotion(Map *map, struct Potion potions[4], int number_of_players)

{
  long long time = currentTime();
  bool players_changed[number_of_players];
  for (int i = 0; i < number_of_players; i++)
    players_changed[i] = false;
  for (struct LinkedListNode *potion_node = map->potions->next; potion_node != NULL; potion_node = potion_node->next)
  {
      //??
    if (potion_node->data == NULL)
      continue;
    PotionNode *potion = potion_node->data;
    SDL_Point *p_potion = &potion->point;
    int potionStatus = potion->potionStatus;
    if (potionStatus >= 5)
      continue;
    for (struct LinkedListNode *soldier_node = map->soldiers->next; soldier_node != NULL; soldier_node = soldier_node->next)
    {
      if (soldier_node->data == NULL)
        continue;
      Soldier *soldier = soldier_node->data;
      SDL_Point point;
      if (usable_function(&point, map, soldier_node, time, false))
      {
        if (is_in_area(point.x, point.y, p_potion->x - R_POTION, p_potion->x + R_POTION, p_potion->y - R_POTION, p_potion->y + R_POTION))
        {
          if (!soldier->player->potion.enabled)
          {
            soldier->player->potion.enabled = true;
            soldier->player->potion.time = currentTime();
            soldier->player->potion.potion = &potions[potionStatus - 1];
            int player_index = findPlayersIndex(soldier->player, map, number_of_players);
            //?? application?
            players_changed[player_index] = true;
            if (potionStatus == 2)
            {
              for (int i = 0; i < number_of_players; i++)
              {
                if (i == player_index)
                  continue;

                map->players[i]->otherPotion.enabled = true;
                map->players[i]->otherPotion.time = currentTime();
                map->players[i]->otherPotion.potion = &potions[potionStatus - 1];
                players_changed[i] = true;
              }
            }
            potion->potionStatus += 4;
            break;
          }
        }
      }
    }
  }

  for (struct LinkedListNode *soldier_node = map->soldiers->next; soldier_node != NULL; soldier_node = soldier_node->next)
  {
    if (soldier_node->data == NULL)
      continue;
    Soldier *soldier = soldier_node->data;
    if (players_changed[findPlayersIndex(soldier->player, map, number_of_players)])
    {
      soldier->potionChanged = true;
    }
  }
}

void is_potion_finished(Map *map, int number_of_players)
{
  long long time = currentTime();
  for (int i = 0; i < number_of_players; i++)
  {
    bool finished = false;
    if (map->players[i]->potion.enabled && time >= map->players[i]->potion.time + map->players[i]->potion.potion->duration)
    {
      map->players[i]->potion.enabled = false;
      finished = true;
    }
    if (map->players[i]->otherPotion.enabled && time >= map->players[i]->otherPotion.time + map->players[i]->otherPotion.potion->duration)
    {
      map->players[i]->otherPotion.enabled = false;
      finished = true;
    }
    if (finished)
    {
        //?? what will happen in the following loop??
      for (struct LinkedListNode *soldier_node = map->soldiers->next; soldier_node != NULL; soldier_node = soldier_node->next)
      {
        if (soldier_node->data == NULL)
          continue;
        Soldier *soldier = soldier_node->data;
        soldier->potionChanged = true;
      }
    }
  }
}

SDL_Texture *potionImages[5];
void initializeImage(SDL_Renderer* sdlRenderer)
{
  IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
  for(int i=1; i<=5; i++)
  {
    char file[20];
    sprintf(file, "potion%d.png", i);
    SDL_Surface* surface = IMG_Load(file);
    potionImages[i-1] = SDL_CreateTextureFromSurface(sdlRenderer, surface);
  }
}

long long lastTimePotion = 0;
void renderPotion(SDL_Renderer *sdlRenderer,  Map *map, int number_of_blocks)
{
  long long time1 = currentTime();
  srand(time(NULL));
  int x = (rand()%4 + 1) * 2000; 
  //??
  if (time1 >= lastTimePotion)
  {
    PotionNode *node = malloc(sizeof(PotionNode));
    node->time = time1;
    randomPotion(map, number_of_blocks, &node->point);
    node->potionStatus = rand() % 4 + 1;
    lastTimePotion = time1 + x;
    linked_list_push(map->potions, linked_list_create(node));
  }
                                      //?? potion?
  struct LinkedListNode *parent = map->soldiers;
  for (struct LinkedListNode *potion_node = map->potions->next; potion_node != NULL; potion_node = potion_node->next)
  {
    if (potion_node->data == NULL)
      continue;
    PotionNode *potion = potion_node->data;
    SDL_Point *point = &potion->point;
    int potionStatus = potion->potionStatus;
    int size = R_POTION * 2;
    SDL_Rect rect = {.x = point->x - size/2, .y = point->y - size/2, .w = size, .h = size};
    extern SDL_Texture* potionImages[5];
    SDL_RenderCopy(sdlRenderer, potionImages[(potionStatus > 5 ? 5 : potionStatus) - 1], NULL, &rect);
                            //>=
    if (potion->time + 10000 <= time1)
    {
      parent->next = potion_node->next;
      free(potion_node->data);
      free(potion_node);
      potion_node = parent;
    }
    parent = potion_node;
  }
}

