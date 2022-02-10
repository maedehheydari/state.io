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

#define HEIGHT 800
#define WIDTH 800
#define HEXAGON_LENGTH 70
#define R_SOLDIER 5
#define R_POTION 20
#define is_in_area(x, y, x1, x2, y1, y2) ((x) >= (x1) && (x) <= (x2) && (y) >= (y1) && (y) <= (y2))
#define LIMITATION_OF_SOLDIERS(playerStatus) (playerStatus == 1 ? 0 : 50)

bool same_point(int x1, int y1, int x2, int y2)
{
  if (((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) <= 100)
    return true;
  else
    return false;
}

bool is_in_button(int x, int y, Rect rect)
{
  if (x >= rect.x1 && x <= rect.x2 && y >= rect.y1 && y <= rect.y2)
    return true;
  return false;
}

void initialize_region_color(Uint32 regionColor[])
{
  regionColor[0] = 0x55ffff33;
  regionColor[1] = 0x55999999;
  regionColor[2] = 0x55ff00ff;
  regionColor[3] = 0x5500ffff;
  regionColor[4] = 0x55056443;
  regionColor[5] = 0x552240ff;
  regionColor[6] = 0x55403020;
  regionColor[7] = 0x55aa3950;
  regionColor[8] = 0x5587bb45;
  regionColor[9] = 0x55123999;
  regionColor[10] = 0x5540aa30;
  regionColor[11] = 0x55ff6020;
  regionColor[12] = 0x55935640;
}

void initialize_sarbazkhune_color(Uint32 sarbazkhuneColor[])
{
  sarbazkhuneColor[0] = 0xffffff33;
  sarbazkhuneColor[1] = 0xff999999;
  sarbazkhuneColor[2] = 0xffff00ff;
  sarbazkhuneColor[3] = 0xff00ffff;
  sarbazkhuneColor[4] = 0xff056443;
  sarbazkhuneColor[5] = 0xff2240ff;
  sarbazkhuneColor[6] = 0xff403020;
  sarbazkhuneColor[7] = 0xffaa3950;
  sarbazkhuneColor[8] = 0xff87bb45;
  sarbazkhuneColor[9] = 0xff123999;
  sarbazkhuneColor[10] = 0xff40aa30;
  sarbazkhuneColor[11] = 0xffff6020;
  sarbazkhuneColor[12] = 0xff935640;
}

void initialize_player(Map *map, int number_of_players, Uint32 regionColor[], Uint32 sarbazkhuneColor[])
{
  map->players = (Player **)malloc(number_of_players * sizeof(Player *));
  for (int y = 0; y < number_of_players; y++)
  {
    Player *newplayer = (Player *)malloc(sizeof(Player));
    newplayer->color = regionColor[y];
    newplayer->sarbazkhuneColor = sarbazkhuneColor[y];
    newplayer->potionStatus = 0;
    newplayer->potion.enabled = false;
    newplayer->otherPotion.enabled = false;
    newplayer->time = currentTime();
    map->players[y] = newplayer;
    if (y == 0 || y == 1)
      map->players[y]->is_opponent = false;
    else
      map->players[y]->is_opponent = true;
  }
}

void initialize_blocks(Map *map, int number_of_players, int n, int seed) // n is a number of blocks here
{
  if (seed == 2)
  {
    for (int i = 0; i < 13; i++)
    {
      map->blocks[i]->status = 0;
      map->blocks[i]->player = map->players[0];
      map->blocks[i]->texture = NULL;
      map->blocks[i]->surface = NULL;
      map->blocks[i]->time = 0;
      map->blocks[i]->font = TTF_OpenFont("padmaa.ttf", 18);
      map->blocks[i]->number_of_soldiers = 10;
    }
    for (int i = 13; i < 14; i++)
    {
      map->blocks[i]->status = 2;
      map->blocks[i]->player = map->players[2];
      map->blocks[i]->texture = NULL;
      map->blocks[i]->surface = NULL;
      map->blocks[i]->font = TTF_OpenFont("padmaa.ttf", 18);
      map->blocks[i]->time = 0;
      map->blocks[i]->number_of_soldiers = 10;
    }
  }
  else
  {
    srand(time(NULL));
    int count = 0;
    while (count < number_of_players)
    {
      int i = rand() % n;
      if (map->blocks[i]->player != NULL)
        continue;
      map->blocks[i]->status = count;
      map->blocks[i]->player = map->players[count];
      map->blocks[i]->texture = NULL;
      map->blocks[i]->surface = NULL;
      map->blocks[i]->time = 0;
      map->blocks[i]->font = TTF_OpenFont("padmaa.ttf", 18);
      map->blocks[i]->number_of_soldiers = 10;
      count++;
    }

    for (int i = 0; i < n; i++)
    {
      if (map->blocks[i]->player != NULL)
        continue;
      map->blocks[i]->status = 1;
      map->blocks[i]->player = map->players[1];
      map->blocks[i]->texture = NULL;
      map->blocks[i]->surface = NULL;
      map->blocks[i]->font = TTF_OpenFont("padmaa.ttf", 18);
      map->blocks[i]->time = 0;
      map->blocks[i]->number_of_soldiers = 10;
    }
  }
}

void free_surface(Block *o)
{
  if (o->surface != NULL)
    SDL_FreeSurface(o->surface);
  if (o->texture != NULL)
    SDL_DestroyTexture(o->texture);
  o->surface = NULL;
  o->texture = NULL;
  o->refresh = false;
}

void render_label(SDL_Renderer *renderer, Block *block, int x, int y)
{
  if (block->refresh)
  {
    block->refresh = false;
    free_surface(block);
  }
  if (block->surface == NULL)
  {
    char *text = malloc(10);
    sprintf(text, "%d", block->number_of_soldiers);
    SDL_Color textColor = {.a = 255, .r = 0, .g = 0, .b = 0};
    block->surface = TTF_RenderText_Blended_Wrapped(block->font, text, textColor, 50);
    block->texture = SDL_CreateTextureFromSurface(renderer, block->surface);
  }
  int w = ((SDL_Surface *)block->surface)->w;
  int h = ((SDL_Surface *)block->surface)->h;
  SDL_Rect rect = {.x = x - w / 2, .y = y - h / 2, .w = w, .h = h};
  SDL_RenderCopy(renderer, block->texture, NULL, &rect);
}

void renderLabelMenu(SDL_Renderer *sdlRenderer, Rect rect, char label[], TTF_Font *font)
{
  int x = (rect.x1 + rect.x2) / 2;
  int y = (rect.y1 + rect.y2) / 2;
  SDL_Color textColor = {.a = 255, .r = 0, .g = 0, .b = 0};
  SDL_Surface *sur = TTF_RenderText_Blended_Wrapped(font, label, textColor, rect.x2 - rect.x1);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, sur);
  int w = sur->w;
  int h = sur->h;
  SDL_Rect rectan = {.x = x - w / 2, .y = y - h / 2, .w = w, .h = h};
  SDL_RenderCopy(sdlRenderer, texture, NULL, &rectan);
  SDL_FreeSurface(sur);
  SDL_DestroyTexture(texture);
}

void renderLabelMenuTextbox(SDL_Renderer *sdlRenderer, Rect rect, char label[], TTF_Font *font, int len)
{
  if (!len)
    return;
  int x = (rect.x1 + rect.x2) / 2;
  int y = (rect.y1 + rect.y2) / 2;
  SDL_Color textColor = {.a = 255, .r = 0, .g = 0, .b = 0};
  SDL_Surface *sur = TTF_RenderText_Solid(font, label, textColor);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, sur);
  int w = sur->w;
  int h = sur->h;
  SDL_Rect rectan = {.x = x - w / 2, .y = y - h / 2, .w = w, .h = h};
  SDL_RenderCopy(sdlRenderer, texture, NULL, &rectan);
  SDL_FreeSurface(sur);
  SDL_DestroyTexture(texture);
}

void renderLabelStart3(SDL_Renderer *sdlRenderer, char label[], TTF_Font *font, int number)
{
  int x = 100 * number;
  int y = 200 * number;
  SDL_Color textColor = {.a = 255, .r = 0, .g = 0, .b = 0};
  SDL_Surface *sur = TTF_RenderText_Blended_Wrapped(font, label, textColor, 50);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, sur);
  int w = sur->w;
  int h = sur->h;
  SDL_Rect rectan = {.x = x - w / 2, .y = y - h / 2, .w = w, .h = h};
  SDL_RenderCopy(sdlRenderer, texture, NULL, &rectan);
  SDL_FreeSurface(sur);
  SDL_DestroyTexture(texture);
}

void hexagon(SDL_Renderer *sdlRenderer, int x, int y, Uint32 color)
{
  boxColor(sdlRenderer, x - sqrt(3) / 2 * HEXAGON_LENGTH - 5, y - HEXAGON_LENGTH / 2 - 5, x + sqrt(3) / 2 * HEXAGON_LENGTH + 5, y + HEXAGON_LENGTH / 2 + 5, 0xffffffff);
  filledTrigonColor(sdlRenderer, x, y - HEXAGON_LENGTH - 5, x - sqrt(3) / 2 * HEXAGON_LENGTH - 5, y - HEXAGON_LENGTH / 2 - 5, x + sqrt(3) / 2 * HEXAGON_LENGTH + 5, y - HEXAGON_LENGTH / 2 - 5, 0xffffffff);
  filledTrigonColor(sdlRenderer, x, y + HEXAGON_LENGTH + 5, x - sqrt(3) / 2 * HEXAGON_LENGTH - 5, y + HEXAGON_LENGTH / 2 + 5, x + sqrt(3) / 2 * HEXAGON_LENGTH + 5, y + HEXAGON_LENGTH / 2 + 5, 0xffffffff);
  boxColor(sdlRenderer, x - sqrt(3) / 2 * HEXAGON_LENGTH, y - HEXAGON_LENGTH / 2 + 1, x + sqrt(3) / 2 * HEXAGON_LENGTH, y + HEXAGON_LENGTH / 2 - 1, color);
  filledTrigonColor(sdlRenderer, x, y - HEXAGON_LENGTH, x - sqrt(3) / 2 * HEXAGON_LENGTH, y - HEXAGON_LENGTH / 2, x + sqrt(3) / 2 * HEXAGON_LENGTH, y - HEXAGON_LENGTH / 2, color);
  filledTrigonColor(sdlRenderer, x, y + HEXAGON_LENGTH, x - sqrt(3) / 2 * HEXAGON_LENGTH, y + HEXAGON_LENGTH / 2, x + sqrt(3) / 2 * HEXAGON_LENGTH, y + HEXAGON_LENGTH / 2, color);
}

void ellipse(SDL_Renderer *sdlRenderer, int x, int y, Uint32 color)
{
  filledEllipseColor(sdlRenderer, x, y, 20, 10, color);
  filledEllipseColor(sdlRenderer, x, y, 10, 20, color);
}

void potionizedSarbazkhune(SDL_Renderer *sdlRenderer, int x, int y, Uint32 color)
{
  boxColor(sdlRenderer, x - 8, y - 10, x + 8, y + 15, color);
  filledEllipseColor(sdlRenderer, x, y - 17, 20, 10, color);
}

void render_map(SDL_Renderer *sdlRenderer, int t, Map *map)
{
  int n = 0;
  while (n < t)
  {
    hexagon(sdlRenderer, map->blocks[n]->x, map->blocks[n]->y, map->players[map->blocks[n]->status]->color);
    render_label(sdlRenderer, map->blocks[n], map->blocks[n]->x, map->blocks[n]->y + 30);
    if (map->blocks[n]->player->potion.enabled)
      potionizedSarbazkhune(sdlRenderer, map->blocks[n]->x, map->blocks[n]->y, map->blocks[n]->player->potion.potion->color);
    else
      ellipse(sdlRenderer, map->blocks[n]->x, map->blocks[n]->y, map->blocks[n]->player->sarbazkhuneColor);
    n++;
  }
}

void initializeStart(bool start[])
{
  for (int i = 0; i < 4; i++)
  {
    if (i == 0)
      start[i] = true;
    else
      start[i] = false;
  }
}

void generateStart4(SDL_Renderer *sdlRenderer, Map **map, int number_of_players, int number_of_blocks, Uint32 regionColor[], Uint32 sarbazkhuneColor[], struct Potion potion[], int seed)
{
  *map = generate_map(number_of_blocks, seed);
  initialize_region_color(regionColor);
  initialize_sarbazkhune_color(sarbazkhuneColor);
  initialize_player(*map, number_of_players, regionColor, sarbazkhuneColor);
  initialize_blocks(*map, number_of_players, number_of_blocks, seed);
  initialize_potions(potion);
  initializeImage(sdlRenderer);
}

void renderStart1(SDL_Renderer *sdlRenderer, SDL_bool *shallExit, bool start[], TTF_Font *font, char text[])
{
  int x, y;
  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
  SDL_RenderClear(sdlRenderer);
  Rect rect[3];
  rect[0].x1 = 250;
  rect[0].y1 = 200;
  rect[0].x2 = 550;
  rect[0].y2 = 275;
  rect[1].x1 = 250;
  rect[1].y1 = 300;
  rect[1].x2 = 550;
  rect[1].y2 = 375;
  rect[2].x1 = 200;
  rect[2].x2 = 600;
  rect[2].y1 = 100;
  rect[2].y2 = 150;
  char label[2][50] = {"New Game", "Standing"};
  static int length = 0;
  Uint32 color[2] = {0xff773311, 0xffffffff};
  boxColor(sdlRenderer, rect[2].x1, rect[2].y1, rect[2].x2, rect[2].y2, color[1]);
  renderLabelMenuTextbox(sdlRenderer, rect[2], text, font, length);
  bool selected[2] = {false, false};
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent))
  {
    switch (sdlEvent.type)
    {
    case SDL_QUIT:
      *shallExit = SDL_TRUE;
      break;

    case SDL_KEYDOWN:
      if (sdlEvent.key.keysym.sym == SDLK_RETURN || sdlEvent.key.keysym.sym == SDLK_KP_ENTER)
      {
        start[0] = false;
        start[1] = true;
      }
      else if (sdlEvent.key.keysym.sym == SDLK_BACKSPACE && length)
      {
        length--;
        text[length] = '\0';
      }
      break;

    case SDL_TEXTINPUT:
      text[length % 50] = *sdlEvent.text.text;
      length++;
      if (length >= 50)
        text[50] = '\0';
      else
        text[length] = '\0';
      break;

    case SDL_MOUSEBUTTONDOWN:
      x = sdlEvent.button.x;
      y = sdlEvent.button.y;
      for (int i = 0; i < 2; i++)
      {
        if (is_in_button(x, y, rect[i]))
        {
          start[0] = false;
          start[i + 1] = true;
        }
      }
    }
  }
  int x1, y1;
  SDL_GetMouseState(&x1, &y1);
  for (int i = 0; i < 2; i++)
  {
    if (is_in_button(x1, y1, rect[i]))
      boxColor(sdlRenderer, rect[i].x1, rect[i].y1, rect[i].x2, rect[i].y2, color[0]);
    else
      boxColor(sdlRenderer, rect[i].x1, rect[i].y1, rect[i].x2, rect[i].y2, color[1]);

    renderLabelMenu(sdlRenderer, rect[i], label[i], font);
  }
  SDL_RenderPresent(sdlRenderer);
}

void renderStart2(SDL_Renderer *sdlRenderer, Map *map, SDL_bool *shallExit, bool start[], TTF_Font *font, int *number_of_players, int *number_of_blocks, Uint32 regionColor[], Uint32 sarbazkhuneColor[], struct Potion potion[], int *seed)
{
  int x, y;
  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
  SDL_RenderClear(sdlRenderer);
  Rect rect[5];
  rect[0].x1 = 250;
  rect[0].y1 = 225;
  rect[0].x2 = 550;
  rect[0].y2 = 300;
  for (int i = 1; i < 5; i++)
  {
    rect[i].x1 = 250;
    rect[i].x2 = 550;
    rect[i].y1 = rect[i - 1].y1 + 100;
    rect[i].y2 = rect[i - 1].y2 + 100;
  }
  char label[5][50] = {"Map1", "Map2", "Map3", "Map4", "Random Map"};
  Uint32 color[2] = {0xff550000, 0xffffff00};
  SDL_Event sdlEvent;
  static bool player = true;
  static int length = 0, length1 = 0;
  static char text[10], text1[10];
  Rect boxrect[2];
  boxrect[0].x1 = 200;
  boxrect[0].y1 = 100;
  boxrect[0].x2 = 600;
  boxrect[0].y2 = 150;
  boxrect[1].x1 = 200;
  boxrect[1].y1 = 160;
  boxrect[1].x2 = 600;
  boxrect[1].y2 = 210;
  boxColor(sdlRenderer, boxrect[0].x1, boxrect[0].y1, boxrect[0].x2, boxrect[0].y2, color[1]);
  renderLabelMenuTextbox(sdlRenderer, boxrect[0], text, font, length);
  boxColor(sdlRenderer, boxrect[1].x1, boxrect[1].y1, boxrect[1].x2, boxrect[1].y2, color[1]);
  renderLabelMenuTextbox(sdlRenderer, boxrect[1], text1, font, length1);
  while (SDL_PollEvent(&sdlEvent))
  {
    switch (sdlEvent.type)
    {
    case SDL_QUIT:
      *shallExit = SDL_TRUE;
      break;

    case SDL_KEYDOWN:
      if (player)
      {
        if (sdlEvent.key.keysym.sym == SDLK_RETURN || sdlEvent.key.keysym.sym == SDLK_KP_ENTER)
          player = false;
        else if (sdlEvent.key.keysym.sym == SDLK_BACKSPACE && length)
        {
          length--;
          text[length] = '\0';
        }
        break;
      }
      else
      {
        if (sdlEvent.key.keysym.sym == SDLK_RETURN || sdlEvent.key.keysym.sym == SDLK_KP_ENTER)
        {
          srand(time(NULL));
          if (!length)
            *number_of_players = rand() % 5 + 5;
          else
            *number_of_players = atoi(text);
          if (!length1)
            *number_of_blocks = rand() % 15 + 15;
          else
            *number_of_blocks = atoi(text1);
          start[1] = false;
          start[3] = true;
        }
        else if (sdlEvent.key.keysym.sym == SDLK_BACKSPACE && length1)
        {
          length1--;
          text1[length1] = '\0';
        }
        break;
      }

    case SDL_TEXTINPUT:
      if (player)
      {
        text[length % 50] = *sdlEvent.text.text;
        length++;
        if (length >= 50)
          text[50] = '\0';
        else
          text[length] = '\0';
        break;
      }
      else
      {
        text1[length1 % 50] = *sdlEvent.text.text;
        length1++;
        if (length1 >= 50)
          text1[50] = '\0';
        else
          text1[length1] = '\0';
        break;
      }

    case SDL_MOUSEBUTTONDOWN:
      x = sdlEvent.button.x;
      y = sdlEvent.button.y;
      for (int i = 0; i < 5; i++)
      {
        if (is_in_button(x, y, rect[i]))
        {
          start[1] = false;
          start[3] = true;
          switch (i)
          {
          case 0:
            *number_of_players = 3;
            *number_of_blocks = 14;
            *seed = 2;
            break;
          case 1:
            *number_of_players = 5;
            *number_of_blocks = 20;
            *seed = 1;
            break;
          case 2:
            *number_of_players = 6;
            *number_of_blocks = 25;
            *seed = 1;
            break;
          case 3:
            *number_of_players = 7;
            *number_of_blocks = 25;
            *seed = 1;
            break;
          case 4:
            srand(time(NULL));
            if (!length)
              *number_of_players = rand() % 5 + 5;
            else
              *number_of_players = atoi(text);
            if (!length1)
              *number_of_blocks = rand() % 15 + 15;
            else
              *number_of_blocks = atoi(text1);
            break;
          }
        }
      }
    }
  }
  int x1, y1;
  SDL_GetMouseState(&x1, &y1);
  for (int i = 0; i < 5; i++)
  {
    if (is_in_button(x1, y1, rect[i]))
      boxColor(sdlRenderer, rect[i].x1, rect[i].y1, rect[i].x2, rect[i].y2, color[0]);
    else
      boxColor(sdlRenderer, rect[i].x1, rect[i].y1, rect[i].x2, rect[i].y2, color[1]);

    renderLabelMenu(sdlRenderer, rect[i], label[i], font);
  }
  SDL_RenderPresent(sdlRenderer);
}

void renderStart3(SDL_Renderer *sdlRenderer, SDL_bool *shallExit, TTF_Font *font)
{
  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
  SDL_RenderClear(sdlRenderer);
  char champion[50], stringhighScore[50] = "hello";
  int highScore;
  readFromTheFile(champion, &highScore);
  boxColor(sdlRenderer, 200, 200, 600, 300, 0xffffffff);
  Rect rect;
  rect.x1 = 220;
  rect.x2 = 500;
  rect.y1 = 210;
  rect.y2 = 290;
  renderLabelMenu(sdlRenderer, rect, champion, font);
  Rect rect1;
  rect1.x1 = 500;
  rect1.x2 = 600;
  rect1.y1 = 210;
  rect1.y2 = 290;
  int count = 0;
  int highScoreCopy = highScore;
  while (highScore)
  {
    highScore /= 10;
    count++;
  }
  for (int i = count - 1; i >= 0; i--)
  {
    stringhighScore[i] = highScoreCopy % 10 + '0';
    highScoreCopy /= 10;
  }
  stringhighScore[count] = '\0';
  renderLabelMenu(sdlRenderer, rect1, stringhighScore, font);
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent))
  {
    switch (sdlEvent.type)
    {
    case SDL_QUIT:
      *shallExit = SDL_TRUE;
      break;
    }
  }
  SDL_RenderPresent(sdlRenderer);
}

void renderStart4(SDL_Renderer *sdlRenderer, Map *map, int number_of_players, int number_of_blocks, SDL_bool *shallExit, struct Potion potion[], char text[], bool start[])
{
  int score;
  static bool touch;
  static int srcIndex = -1, destIndex = -1;
  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
  SDL_RenderClear(sdlRenderer);
  SDL_Event sdlEvent;
  generate_soldier(map, number_of_blocks);
  AI(map, number_of_players, number_of_blocks);
  render_map(sdlRenderer, number_of_blocks, map);
  ellipse(sdlRenderer, map->blocks[0]->x, map->blocks[0]->y, 1);
  while (SDL_PollEvent(&sdlEvent))
  {
    switch (sdlEvent.type)
    {

    case SDL_QUIT:
      *shallExit = SDL_TRUE;
      break;

    case SDL_MOUSEBUTTONDOWN:
      // if (srcIndex == -1)
      // {
      //   srcIndex = find_the_block(map, number_of_blocks, sdlEvent.button.x, sdlEvent.button.y);
      //   if (srcIndex == -1)
      //     srcIndex = -1;
      //   touch = srcIndex != -1;
      // }
      if (srcIndex == -1)
      {
        srcIndex = find_the_block(map, number_of_blocks, sdlEvent.button.x, sdlEvent.button.y);
        if(srcIndex != -1 && map -> blocks[srcIndex]->status != 0)
          srcIndex = 1;
        touch = srcIndex != -1;
      }
      else
      {
        destIndex = find_the_block(map, number_of_blocks, sdlEvent.button.x, sdlEvent.button.y);
        if (srcIndex == destIndex)
          srcIndex = destIndex = -1;
        touch = destIndex != -1;
      }

    case SDL_MOUSEBUTTONUP:
      if (touch)
      {
        if (destIndex == -1)
        {
          int a = find_the_block(map, number_of_blocks, sdlEvent.button.x, sdlEvent.button.y);
          if (a != srcIndex)
            srcIndex = -1;
        }
        else
        {
          int a = find_the_block(map, number_of_blocks, sdlEvent.button.x, sdlEvent.button.y);
          if (a != destIndex || ((map->blocks[a]->player->potion.enabled && map->blocks[a]->player->potion.potion->status == 3) && (map->blocks[a]->status != map->blocks[srcIndex]->status)))
            destIndex = -1;
          else if (a != -1)
            move_soldier(map, srcIndex, destIndex);
          srcIndex = destIndex = -1;
        }
      }
      touch = false;
    }
  }

  is_potion_finished(map, number_of_players);
  render_soldier(sdlRenderer, map);
  renderPotion(sdlRenderer, map, number_of_blocks);
  conflictWithPotion(map, potion, number_of_players);
  score = is_finished_win(map, number_of_blocks, number_of_players);
  if (score)
  {
    start[3] = false;
   start[4] = true;
   writeInFile(text, score);
  }
  else
  {
    score = is_finished_lose(map, number_of_blocks, number_of_players);
    if (score != 0)
    {
      start[3] = false;
      start[5] = true;
     writeInFile(text, score);
    }
  }
  SDL_RenderPresent(sdlRenderer);
}

void renderStart5(SDL_Renderer *sdlRenderer, Map *map, int number_of_players, int number_of_blocks, SDL_bool *shallExit, struct Potion potion[], char text[], bool start[], TTF_Font *font)
{
  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 255, 255);
  SDL_RenderClear(sdlRenderer);
  SDL_Event sdlEvent;
  char text1[50] = "You won";
  boxColor(sdlRenderer, 200, 200, 600, 300, 0xffffffff);
  Rect rect;
  rect.x1 = 220;
  rect.x2 = 580;
  rect.y1 = 210;
  rect.y2 = 290;
  renderLabelMenu(sdlRenderer, rect, text1, font);
  while (SDL_PollEvent(&sdlEvent))
  {
    switch (sdlEvent.type)
    {
    case SDL_QUIT:
      *shallExit = SDL_TRUE;
      break;
    }
  }
  SDL_RenderPresent(sdlRenderer);
}

void renderStart6(SDL_Renderer *sdlRenderer, Map *map, int number_of_players, int number_of_blocks, SDL_bool *shallExit, struct Potion potion[], char text[], bool start[], TTF_Font *font)
{
  SDL_SetRenderDrawColor(sdlRenderer, 255, 0, 0, 255);
  SDL_RenderClear(sdlRenderer);
  SDL_Event sdlEvent;
  char text1[50] = "You lost";
  boxColor(sdlRenderer, 200, 200, 600, 300, 0xffffffff);
  Rect rect;
  rect.x1 = 220;
  rect.x2 = 580;
  rect.y1 = 210;
  rect.y2 = 290;
  renderLabelMenu(sdlRenderer, rect, text1, font);
  while (SDL_PollEvent(&sdlEvent))
  {
    switch (sdlEvent.type)
    {
    case SDL_QUIT:
      *shallExit = SDL_TRUE;
      break;
    }
  }
  SDL_RenderPresent(sdlRenderer);
}

void readFromTheFile(char champion[], int *highScore)
{
  *highScore = -1000000;
  FILE *f1 = fopen("standing.txt", "r");
  if (f1 != NULL)
  {
    while (!feof(f1))
    {
      char n[50];
      int score;
      char sign;
      if (fscanf(f1, "%[^\n]%*c%c%d%*c", n, &sign, &score))
      {
        if (sign == '-' && score >= 0)
          score *= -1;
        if (score >= *highScore)
        {
          strcpy(champion, n);
          *highScore = score;
        }
      }
    }
  }
  fclose(f1);
}

void writeInFile(char name[], int newScore)
{
  FILE *f1 = fopen("standing.txt", "r");
  bool find = false;
  int s = 0, l = 1, oldS = 0;
  int capacity = 64;
  char *file = malloc(capacity);
  if (f1 != NULL)
  {
    while (!feof(f1))
    {
      file = realloc(file, (l++) * capacity);
      char n[50];
      int score;
      char sign;
      if (fscanf(f1, "%[^\n]%*c%c%d%*c", n, &sign, &score))
      {
        if (sign == '-')
          score *= -1;

        if (!find && strcmp(n, name) == 0)
        {
          find = true;
          score += newScore;
        }
        oldS = s;
        strcpy(file + s, n);
        s += strlen(n);
        file[s++] = '\n';
        sprintf(file + s, "%c%d\n", score >= 0 ? '+' : '-', score < 0 ? -score : score);
        s += (score == 0 ? 1 : (int)log10(score < 0 ? -score : score)) + 3;
      }
    }
  }
  file[oldS] = '\0';

  if (f1 != NULL)
    fclose(f1);
  if (find)
  {
    FILE *f2 = fopen("standing.txt", "w");
    fprintf(f2, "%s", file);
    fclose(f2);
  }
  else
  {
    FILE *f2 = fopen("standing.txt", "a");
    fprintf(f2, "%s\n%c%d\n", name, newScore >= 0 ? '+' : '-', newScore < 0 ? -newScore : newScore);
    fclose(f2);
  }
  free(file);
}

Map *generate_map(int n, int seed)
{
  Map *map = (Map *)malloc(sizeof(Map));
  map->soldiers = linked_list_create(NULL);
  map->potions = linked_list_create(NULL);
  map->blocks = NULL;
  map->players = NULL;
  int regions_number = 0;
  Block *block = NULL;
  if (seed)
    srand(1000);
  else
    srand(time(NULL));
  while (regions_number < n)
  {
    if (block == NULL && regions_number == 0)
    {
      block = (Block *)malloc(sizeof(Block));
      block->x = WIDTH / 2;
      block->y = HEIGHT / 2;
      block->number_of_soldiers = 0;
      block->player = NULL;
      block->east = NULL;
      block->west = NULL;
      block->north_east = NULL;
      block->south_east = NULL;
      block->north_west = NULL;
      block->south_west = NULL;
      map->blocks = (Block **)malloc(n * sizeof(Block));
      map->blocks[regions_number] = block;
      regions_number++;
      continue;
    }
    int rnd = rand() % 6 + 1;
    switch (rnd)
    {
    case (1):
      if (block->east == NULL && block->x + sqrt(3) * 3.0 / 2.0 * HEXAGON_LENGTH < WIDTH)
      {
        Block *newBlock = (Block *)malloc(sizeof(Block));
        newBlock->x = block->x + sqrt(3) * HEXAGON_LENGTH;
        newBlock->y = block->y;
        newBlock->number_of_soldiers = 0;
        newBlock->player = NULL;
        newBlock->east = NULL;
        newBlock->west = NULL;
        newBlock->north_east = NULL;
        newBlock->south_east = NULL;
        newBlock->north_west = NULL;
        newBlock->south_west = NULL;
        for (int i = 0; i < regions_number; i++)
        {
          if (same_point(newBlock->x + sqrt(3) * HEXAGON_LENGTH, newBlock->y, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->east = map->blocks[i];
            map->blocks[i]->west = newBlock;
          }
          else if (same_point(newBlock->x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y - HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->north_east = map->blocks[i];
            map->blocks[i]->south_west = newBlock;
          }
          else if (same_point(newBlock->x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y + HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->south_east = map->blocks[i];
            map->blocks[i]->north_west = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) * HEXAGON_LENGTH, newBlock->y, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->west = map->blocks[i];
            map->blocks[i]->east = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y - HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->north_west = map->blocks[i];
            map->blocks[i]->south_east = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y + HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->south_west = map->blocks[i];
            map->blocks[i]->north_east = newBlock;
          }
        }
        block = newBlock;
        map->blocks[regions_number] = block;
        regions_number++;
      }
      else
      {
        int rnd_region = rand() % regions_number;
        block = map->blocks[rnd_region];
      }
      break;

    case (2):
      if (block->south_east == NULL && block->x + sqrt(3) * HEXAGON_LENGTH < WIDTH && block->y + HEXAGON_LENGTH * 5.0 / 2.0 < HEIGHT)
      {
        Block *newBlock = (Block *)malloc(sizeof(Block));
        newBlock->x = block->x + sqrt(3) / 2 * HEXAGON_LENGTH;
        newBlock->y = block->y + HEXAGON_LENGTH * 3.0 / 2.0;
        newBlock->number_of_soldiers = 0;
        newBlock->player = NULL;
        newBlock->east = NULL;
        newBlock->west = NULL;
        newBlock->north_east = NULL;
        newBlock->south_east = NULL;
        newBlock->north_west = NULL;
        newBlock->south_west = NULL;
        for (int i = 0; i < regions_number; i++)
        {
          if (same_point(newBlock->x + sqrt(3) * HEXAGON_LENGTH, newBlock->y, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->east = map->blocks[i];
            map->blocks[i]->west = newBlock;
          }
          else if (same_point(newBlock->x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y - HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->north_east = map->blocks[i];
            map->blocks[i]->south_west = newBlock;
          }
          else if (same_point(newBlock->x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y + HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->south_east = map->blocks[i];
            map->blocks[i]->north_west = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) * HEXAGON_LENGTH, newBlock->y, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->west = map->blocks[i];
            map->blocks[i]->east = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y - HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->north_west = map->blocks[i];
            map->blocks[i]->south_east = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y + HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->south_west = map->blocks[i];
            map->blocks[i]->north_east = newBlock;
          }
        }
        block = newBlock;
        map->blocks[regions_number] = block;
        regions_number++;
      }
      else
      {
        int rnd_region = rand() % regions_number;
        block = map->blocks[rnd_region];
      }
      break;

    case (3):
      if (block->south_west == NULL && block->x - sqrt(3) * HEXAGON_LENGTH > 0 && block->y + HEXAGON_LENGTH * 5.0 / 2.0 < HEIGHT)
      {
        Block *newBlock = (Block *)malloc(sizeof(Block));
        newBlock->x = block->x - sqrt(3) / 2 * HEXAGON_LENGTH;
        newBlock->y = block->y + HEXAGON_LENGTH * 3.0 / 2.0;
        newBlock->number_of_soldiers = 0;
        newBlock->player = NULL;
        newBlock->east = NULL;
        newBlock->west = NULL;
        newBlock->north_east = NULL;
        newBlock->south_east = NULL;
        newBlock->north_west = NULL;
        newBlock->south_west = NULL;
        for (int i = 0; i < regions_number; i++)
        {
          if (same_point(newBlock->x + sqrt(3) * HEXAGON_LENGTH, newBlock->y, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->east = map->blocks[i];
            map->blocks[i]->west = newBlock;
          }
          else if (same_point(newBlock->x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y - HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->north_east = map->blocks[i];
            map->blocks[i]->south_west = newBlock;
          }
          else if (same_point(newBlock->x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y + HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->south_east = map->blocks[i];
            map->blocks[i]->north_west = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) * HEXAGON_LENGTH, newBlock->y, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->west = map->blocks[i];
            map->blocks[i]->east = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y - HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->north_west = map->blocks[i];
            map->blocks[i]->south_east = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y + HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->south_west = map->blocks[i];
            map->blocks[i]->north_east = newBlock;
          }
        }
        block = newBlock;
        map->blocks[regions_number] = block;
        regions_number++;
      }
      else
      {
        int rnd_region = rand() % regions_number;
        block = map->blocks[rnd_region];
      }
      break;

    case (4):
      if (block->west == NULL && block->x - sqrt(3) * 3.0 / 2.0 * HEXAGON_LENGTH > 0)
      {
        Block *newBlock = (Block *)malloc(sizeof(Block));
        newBlock->x = block->x - sqrt(3) * HEXAGON_LENGTH;
        newBlock->y = block->y;
        newBlock->number_of_soldiers = 0;
        newBlock->player = NULL;
        newBlock->east = NULL;
        newBlock->west = NULL;
        newBlock->north_east = NULL;
        newBlock->south_east = NULL;
        newBlock->north_west = NULL;
        newBlock->south_west = NULL;
        for (int i = 0; i < regions_number; i++)
        {
          if (same_point(newBlock->x + sqrt(3) * HEXAGON_LENGTH, newBlock->y, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->east = map->blocks[i];
            map->blocks[i]->west = newBlock;
          }
          else if (same_point(newBlock->x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y - HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->north_east = map->blocks[i];
            map->blocks[i]->south_west = newBlock;
          }
          else if (same_point(newBlock->x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y + HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->south_east = map->blocks[i];
            map->blocks[i]->north_west = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) * HEXAGON_LENGTH, newBlock->y, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->west = map->blocks[i];
            map->blocks[i]->east = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y - HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->north_west = map->blocks[i];
            map->blocks[i]->south_east = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y + HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->south_west = map->blocks[i];
            map->blocks[i]->north_east = newBlock;
          }
        }
        block = newBlock;
        map->blocks[regions_number] = block;
        regions_number++;
      }
      else
      {
        int rnd_region = rand() % regions_number;
        block = map->blocks[rnd_region];
      }
      break;

    case (5):
      if (block->north_west == NULL && block->x - sqrt(3) * HEXAGON_LENGTH > 0 && block->y - HEXAGON_LENGTH * 5.0 / 2.0 > 0)
      {
        Block *newBlock = (Block *)malloc(sizeof(Block));
        newBlock->x = block->x - sqrt(3) / 2 * HEXAGON_LENGTH;
        newBlock->y = block->y - HEXAGON_LENGTH * 3.0 / 2.0;
        newBlock->number_of_soldiers = 0;
        newBlock->player = NULL;
        newBlock->east = NULL;
        newBlock->west = NULL;
        newBlock->north_east = NULL;
        newBlock->south_east = NULL;
        newBlock->north_west = NULL;
        newBlock->south_west = NULL;
        for (int i = 0; i < regions_number; i++)
        {
          if (same_point(newBlock->x + sqrt(3) * HEXAGON_LENGTH, newBlock->y, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->east = map->blocks[i];
            map->blocks[i]->west = newBlock;
          }
          else if (same_point(newBlock->x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y - HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->north_east = map->blocks[i];
            map->blocks[i]->south_west = newBlock;
          }
          else if (same_point(newBlock->x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y + HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->south_east = map->blocks[i];
            map->blocks[i]->north_west = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) * HEXAGON_LENGTH, newBlock->y, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->west = map->blocks[i];
            map->blocks[i]->east = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y - HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->north_west = map->blocks[i];
            map->blocks[i]->south_east = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y + HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->south_west = map->blocks[i];
            map->blocks[i]->north_east = newBlock;
          }
        }
        block = newBlock;
        map->blocks[regions_number] = block;
        regions_number++;
      }
      else
      {
        int rnd_region = rand() % regions_number;
        block = map->blocks[rnd_region];
      }
      break;

    case (6):
      if (block->north_east == NULL && block->x + sqrt(3) * HEXAGON_LENGTH < WIDTH && block->y - HEXAGON_LENGTH * 5.0 / 2.0 > 0)
      {
        Block *newBlock = (Block *)malloc(sizeof(Block));
        newBlock->x = block->x + sqrt(3) / 2 * HEXAGON_LENGTH;
        newBlock->y = block->y - HEXAGON_LENGTH * 3.0 / 2.0;
        newBlock->number_of_soldiers = 0;
        newBlock->player = NULL;
        newBlock->east = NULL;
        newBlock->west = NULL;
        newBlock->north_east = NULL;
        newBlock->south_east = NULL;
        newBlock->north_west = NULL;
        newBlock->south_west = NULL;
        for (int i = 0; i < regions_number; i++)
        {
          if (same_point(newBlock->x + sqrt(3) * HEXAGON_LENGTH, newBlock->y, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->east = map->blocks[i];
            map->blocks[i]->west = newBlock;
          }
          else if (same_point(newBlock->x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y - HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->north_east = map->blocks[i];
            map->blocks[i]->south_west = newBlock;
          }
          else if (same_point(newBlock->x + sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y + HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->south_east = map->blocks[i];
            map->blocks[i]->north_west = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) * HEXAGON_LENGTH, newBlock->y, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->west = map->blocks[i];
            map->blocks[i]->east = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y - HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->north_west = map->blocks[i];
            map->blocks[i]->south_east = newBlock;
          }
          else if (same_point(newBlock->x - sqrt(3) / 2 * HEXAGON_LENGTH, newBlock->y + HEXAGON_LENGTH * 3.0 / 2.0, map->blocks[i]->x, map->blocks[i]->y))
          {
            newBlock->south_west = map->blocks[i];
            map->blocks[i]->north_east = newBlock;
          }
        }
        block = newBlock;
        map->blocks[regions_number] = block;
        regions_number++;
      }
      else
      {
        int rnd_region = rand() % regions_number;
        block = map->blocks[rnd_region];
      }
      break;
    }
  }
  return map;
};