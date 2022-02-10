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
#define is_in_area(x, y, x1, x2, y1, y2) ((x) >= (x1) && (x) <= (x2) && (y) >= (y1) && (y) <= (y2))
#define LIMITATION_OF_SOLDIERS(playerStatus) (playerStatus == 1 ? 0 : 50)

long long currentTime()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  //??
  return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

int find_the_block(Map *map, int number_of_blocks, int x, int y)
{
  for (int i = 0; i < number_of_blocks; i++)
  {
    if ((x < map->blocks[i]->x + 60 && x > map->blocks[i]->x - 60) && (y < map->blocks[i]->y + 60 && y > map->blocks[i]->y - 60))
      return i;
  }
  return -1;
}

long long lastTime = 0;
void generate_soldier(Map *map, int number_of_blocks)
{
  long long time = currentTime();
  if (time >= lastTime + 1000)
  {
    for (int i = 0; i < number_of_blocks; i++)
    {
      if (map->blocks[i]->number_of_soldiers >= LIMITATION_OF_SOLDIERS(map->blocks[i]->status))
        continue;
      if (map->blocks[i]->player->potion.enabled && map->blocks[i]->player->potion.potion->status == 4)
        map->blocks[i]->number_of_soldiers += 2;
      else
        map->blocks[i]->number_of_soldiers++;
      map->blocks[i]->refresh = true;
    }
    lastTime = time + 1000;
  }
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

bool usable_function(SDL_Point *point, Map *map, struct LinkedListNode *soldier_node, long long time, bool render)
{
  if (soldier_node->data == NULL)
    return false;
  Soldier *soldier = soldier_node->data;
  if (render && soldier->potionChanged)
  {
    soldier->potionChanged = false;
    soldier->time = time;
    soldier->src.x = soldier->x;
    soldier->src.y = soldier->y;
  }
  int x = soldier->src.x;
  int y = soldier->src.y;
  long long passed = time - soldier->time;
  if (passed < 0 || soldier->time == -1)
    return false;
  else
  {
    if (render && !soldier->moved)
    {
      if (map->blocks[soldier->srcIndex]->player == soldier->player && map->blocks[soldier->srcIndex]->number_of_soldiers > 0)
      {
        map->blocks[soldier->srcIndex]->number_of_soldiers--;
        map->blocks[soldier->srcIndex]->refresh = true;
        soldier->moved = true;
      }
      else
      {
        soldier->time = -1;
        return false;
      }
    }
  }
  int duration = (int)sqrt(pow(x - soldier->dest.x, 2) + pow(y - soldier->dest.y, 2)) * 10;
  if (soldier->player->potion.enabled)
  {
    if (soldier->player->potion.potion->status == 1)
      duration /= 3;
  }
  if (soldier->player->otherPotion.enabled)
  {
    if (soldier->player->otherPotion.potion->status == 2)
      duration *= 3;
  }
  double scale = (double)passed / duration;
  x += (int)((soldier->dest.x - x) * scale);
  y += (int)((soldier->dest.y - y) * scale);
  if (passed > duration)
  {
    if (render)
    {
      soldier->time = -1;
      if (map->blocks[soldier->destIndex]->player == soldier->player)
        map->blocks[soldier->destIndex]->number_of_soldiers++;
      else
      {
        if (map->blocks[soldier->destIndex]->number_of_soldiers == 0)
        {
          map->blocks[soldier->destIndex]->player = soldier->player;
          map->blocks[soldier->destIndex]->status = soldier->status;
          map->blocks[soldier->destIndex]->number_of_soldiers++;
        }
        else
          map->blocks[soldier->destIndex]->number_of_soldiers--;
      }
      map->blocks[soldier->destIndex]->refresh = true;
    }
    return false;
  }
  point->x = x;
  point->y = y;
  soldier->x = x;
  soldier->y = y;
  return true;
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

void merge_soldiers(Map *map)
{
  long long time = currentTime();

  for (struct LinkedListNode *soldier_node = map->soldiers->next; soldier_node != NULL; soldier_node = soldier_node->next)
  {
    if (soldier_node->data == NULL)
      continue;
    Soldier *soldier = soldier_node->data;
    SDL_Point point;
    if (usable_function(&point, map, soldier_node, time, false))
      for (struct LinkedListNode *soldier_node2 = map->soldiers->next->next; soldier_node2 != NULL; soldier_node2 = soldier_node2->next)
      {
        SDL_Point point2;
        if (usable_function(&point2, map, soldier_node2, time, false))
        {
          Soldier *soldier2 = soldier_node2->data;
          if (soldier2->player != soldier->player)
          {
            if (is_in_area(point2.x, point2.y, point.x - R_SOLDIER, point.x + R_SOLDIER, point.y - R_SOLDIER, point.y + R_SOLDIER))
            {
              soldier->time = -1;
              soldier2->time = -1;
            }
          }
        }
      }
  }
}

void render_soldier(SDL_Renderer *SDL_Renderer, Map *map)
{
  merge_soldiers(map);
  long long time = currentTime();
  struct LinkedListNode *parent = map->soldiers;
  for (struct LinkedListNode *soldier_node = map->soldiers->next; soldier_node != NULL; soldier_node = soldier_node->next)
  {
    if (soldier_node->data == NULL)
      continue;
    Soldier *soldier = soldier_node->data;
    SDL_Point point;
    if (usable_function(&point, map, soldier_node, time, true))
      filledCircleColor(SDL_Renderer, point.x, point.y, R_SOLDIER, soldier->player->sarbazkhuneColor);
    else if (soldier->time == -1)
    {
      parent->next = soldier_node->next;
      free(soldier_node->data);
      free(soldier_node);
      soldier_node = parent;
    }
    parent = soldier_node;
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

int is_finished_win( Map *map, int number_of_blocks, int number_of_players)
{
  int score = 0;
  long long time = currentTime();
  for (int i = 0; i < number_of_blocks; i++)
  {
    if (!(map->blocks[i]->status == 0 || map->blocks[i]->status == 1))
      return score;
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
