#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <math.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL2_gfxPrimitives.h>

#include <SDL2/SDL_ttf.h>

#include <SDL2/SDL_image.h>

#include <time.h>

#include <sys/time.h>

#include "../include/map.h"

#include "../include/linkedlist.h"

#include "../include/player.h"

#define HEIGHT 800

#define WIDTH 800

#define HEXAGON_LENGTH 70

#define R_SARBAZKHUNEH 15

#define R_POTION 20

#define R_SOLDIER 5


long long currentTime()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  //??
  return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

void fix_position(SDL_Point *src, int x, int y, int x2, int y2, int index, int count, bool fromX1)
{
  int c2 = (int)ceil(count / 4.0);
  int i2 = (int)ceil((index + 1) / 4.0);
  if (c2 == i2)
  {
    count %= 4;
    if (count == 0)
      count = 4;
  }
  int row = 4 < count ? 4 : count;
  index %= row;
  int len = 10;
  int space = 7;
  int full = (int)(len * row + space * (row - 1));
  if (y2 == y)
  {
    int a = (int)(y - (full / 2));
    src->y = (int)(a + index * space + index * len);
    return;
  }
  double m = -(double)(x2 - x) / (y2 - y);
  double c = y - m * x;
  double L = full * full / 4;
  int a = (-(-2 * x - 2 * y * m + 2 * m * c) + sqrt(pow(-2 * x - 2 * y * m + 2 * m * c, 2) - 4 * (m * m + 1) * (x * x + y * y - 2 * y * c + c * c - L))) / 2.0 / (m * m + 1);
  int b = m * a + c;
  int a2 = (-(-2 * x - 2 * y * m + 2 * m * c) - sqrt(pow(-2 * x - 2 * y * m + 2 * m * c, 2) - 4 * (m * m + 1) * (x * x + y * y - 2 * y * c + c * c - L))) / 2.0 / (m * m + 1);
  int b2 = m * a2 + c;
  //??
  int spaceX = space * (fabs(a2 - a)) / full;
  //??
  int spaceY = space * (fabs(b2 - b)) / full;
  //??
  int lenX = len * (fabs(a2 - a)) / full;
  //??
  int lenY = len * (fabs(b2 - b)) / full;
  //??
  src->x = (int)(a - index * lenX - index * spaceX);
  //??
  src->y = (int)(b + (m <= 0 ? 1 : -1) * (index * lenY + index * spaceY));
}

void move_soldier(Map *map, int srcIndex, int destIndex)
{
  int count = map->blocks[srcIndex]->number_of_soldiers;
  for (int i = 0; i < count; i++)
  {
    Soldier *newsoldier = malloc(sizeof(Soldier));
    newsoldier->time = currentTime() + (i / 4) * 500;
    newsoldier->src.x = map->blocks[srcIndex]->x;
    newsoldier->src.y = map->blocks[srcIndex]->y;
    newsoldier->dest.x = map->blocks[destIndex]->x;
    newsoldier->dest.y = map->blocks[destIndex]->y;
    newsoldier->srcIndex = srcIndex;
    newsoldier->destIndex = destIndex;
    newsoldier->moved = false;
    newsoldier->status = map->blocks[srcIndex]->status;
    newsoldier->player = map->blocks[srcIndex]->player;
    newsoldier->potionChanged = false;
    //how can it manage the movement of soldiers??
    //&newsodier->src application??
    fix_position(&newsoldier->src, newsoldier->src.x, newsoldier->src.y, newsoldier->dest.x, newsoldier->dest.y, i, count, false);
    newsoldier->x = newsoldier->src.x;
    newsoldier->y = newsoldier->src.y;
    //??
    linked_list_push(map->soldiers, linked_list_create(newsoldier));
    if (i == 0)
    {
      int duration = (int)sqrt(pow(newsoldier->src.x - newsoldier->dest.x, 2) + pow(newsoldier->src.y - newsoldier->dest.y, 2)) * 10;
      int clmn = (int)ceil(count / 4.0);
      //??
      map->blocks[destIndex]->time = currentTime() + clmn * duration + (clmn - 1) * 500;
    }
  }
}

void AI(Map *map, int number_of_players, int number_of_blocks)
{
  long long time = currentTime();
  for (int i = 2; i < number_of_players; i++)
  {
    if (map->players[i]->time + 4000 <= time)
    {
      map->players[i]->time = time;
      for (int j = 0; j < number_of_blocks; j++)
      {
        bool ignore = false;
        if (map->blocks[j]->player == map->players[i])
        {
          for (int k = 0; k < number_of_blocks; k++)
          {
            if (map->blocks[k]->player != map->players[i] && map->blocks[j]->number_of_soldiers >= map->blocks[k]->number_of_soldiers + 4)
            {
              if (map->blocks[k]->time >= time || (map->blocks[k]->player->potion.enabled && map->blocks[k]->player->potion.potion->status == 3))
                continue;
              move_soldier(map, j, k);
              ignore = true;
              break;
            }
          }
        }
        if (ignore)
          break;
      }
    }
  }
}

int is_finished_win( Map *map, int number_of_blocks, int number_of_players)
{
  int score = 0;
  long long time = currentTime();
  for (int i = 0; i < number_of_blocks; i++)
  {
    if (!(map->blocks[i]->status == 0 || map->blocks[i]->status == 1))
    {
      // if(time - begginingTime >= 360000)
      //   score += (360 - (time - begginingTime) / 1000) * 2;
      //return score;
      return score;
    }
  }
   score = number_of_players * 100;
  return score;
}

int is_finished_lose( Map *map, int number_of_blocks, int number_of_players)
{
  int score = 0;
  for (int i = 0; i < number_of_blocks; i++)
  {
    if (map->blocks[i]->status == 0)
      return score;
  }
  score = (13 - number_of_players) * -50;
  return score;
}

int find_the_block(Map *map, int number_of_blocks, int x, int y)
{
  for (int i = 0; i < number_of_blocks; i++)
  {
    if ((x < map->blocks[i]->x + 60 && x > map->blocks[i]->x - 60) && (y < map->blocks[i]->y + 60 && y > map->blocks[i]->y - 60))
    {
      return i;
    }
  }
  return -1;
}

bool find_the_block_for_potion(Map *map, int number_of_blocks, int x, int y)
{
  for (int i = 0; i < number_of_blocks; i++)
  {
    if ((x < map->blocks[i]->x + HEXAGON_LENGTH * 1.0 * sqrt(3) / 2 && x > map->blocks[i]->x - HEXAGON_LENGTH * 1.0 * sqrt(3) / 2) && (y < map->blocks[i]->y + HEXAGON_LENGTH * 1.0 * sqrt(3) / 2 && y > map->blocks[i]->y - HEXAGON_LENGTH * 1.0 * sqrt(3) / 2))
    {
      return true;
    }
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
  //random potion time??
  //int x = (rand()%4 + 1) * 2000; 
  int x= 3000; 
  if (time1 >= lastTimePotion + x)
  {
    PotionNode *node = malloc(sizeof(PotionNode));
    node->time = time1;
    randomPotion(map, number_of_blocks, &node->point);
    node->potionStatus = rand() % 4 + 1;
    lastTimePotion = time1 + x;
    linked_list_push(map->potions, linked_list_create(node));
  }
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

/*void initialize_x_y_soldiers(Map *map, SDL_Renderer sdlRenderer, int number_of_players)
{
  int r = 5;
  int distance = 3;
  for(int i=0; i<number_of_players; i++)
  {
    for(int j=0; j< map->players[i]->number_of_grohans; j++ )
    {
      int col = map->players[i]->grohan[j]->number_of_soldiers / row;
      int t = 3;
      for(int k=0; k<col; k++)
      {
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->x = map->players[i]->grohan[j]->source->x + (map->players[i]->grohan[j]->destination->x - map->players[i]->grohan[j]->source->x) * t;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->y = map->players[i]->grohan[j]->source->y + (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * t;
       
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-1]->x = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->x - (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * distance;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-1]->y = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->y - (map->players[i]->grohan[j]->source->x - map->players[i]->grohan[j]->destination->x) * distance;

        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-2]->x = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->x - (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * distance*2;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-2]->y = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->y - (map->players[i]->grohan[j]->source->x - map->players[i]->grohan[j]->destination->x) * distance*2;

        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1+1]->x = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->x + (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * distance;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1+1]->y = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->y + (map->players[i]->grohan[j]->source->x - map->players[i]->grohan[j]->destination->x) * distance;

        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1+2]->x = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->x + (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * distance*2;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1+2]->y = map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->y + (map->players[i]->grohan[j]->source->x - map->players[i]->grohan[j]->destination->x) * distance*2;

        t -= 3;

      }
      int mod = map->players[i]->grohan[j]->number_of_soldiers % row;
      int temp_x = map->players[i]->grohan[j]->source->x + (map->players[i]->grohan[j]->destination->x - map->players[i]->grohan[j]->source->x) * t;
      int temp_y = map->players[i]->grohan[j]->source->y + (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * t;
      if(mod > 0)
      {
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-2]->x = temp_x - (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * distance*2;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-2]->y = temp_y - (map->players[i]->grohan[j]->source->x - map->players[i]->grohan[j]->destination->x) * distance*2;
      }
      if(mod>1)
      {
         map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-1]->x = temp_x - (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * distance;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1-1]->y = temp_y - (map->players[i]->grohan[j]->source->x - map->players[i]->grohan[j]->destination->x) * distance;

      }
      if(mod>2)
      {
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->x = temp_x
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1]->y = temp_y;
      }
      if(mod>3)
      {
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1+1]->x = temp_x + (map->players[i]->grohan[j]->destination->y - map->players[i]->grohan[j]->source->y) * distance;
        map->players[i]->grohan[j]->soldier[r*(k+1) - r/2 -1+1]->y = temp_y + (map->players[i]->grohan[j]->source->x - map->players[i]->grohan[j]->destination->x) * distance;

      }
    }
  }
}*/
 