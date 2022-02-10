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
        if (length != 0)
        {
          start[0] = false;
          start[1] = true;
        }
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
      if (is_in_button(x, y, rect[0]) && length != 0)
      {
        start[0] = false;
        start[1] = true;
      }
      if (is_in_button(x, y, rect[1]))
      {
        start[0] = false;
        start[2] = true;
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
  char champion[50], stringhighScore[50];
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
  if (highScoreCopy < 0)
  {
    for (int i = count; i >= 1; i--)
    {
      int temp = highScoreCopy % 10;
      if (temp < 0)
        temp *= -1;
      stringhighScore[i] = temp + '0';
      highScoreCopy /= 10;
    }
    stringhighScore[0] = '-';
    stringhighScore[count + 1] = '\0';
  }
  else
  {
    for (int i = count - 1; i >= 0; i--)
    {
      int temp = highScoreCopy % 10;
      if (temp < 0)
        temp *= -1;
      stringhighScore[i] = temp + '0';
      highScoreCopy /= 10;
      printf("%d : %c, highscoreCopy: %d\n", i, stringhighScore[i], highScoreCopy);
    }
    stringhighScore[count] = '\0';
  }
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

void renderStart5(SDL_Renderer *sdlRenderer, Map *map, int number_of_players, int number_of_blocks, SDL_bool *shallExit, struct Potion potion[], char text[], bool start[], TTF_Font *font)
{
  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 255, 255);
  SDL_RenderClear(sdlRenderer);
  SDL_Event sdlEvent;
  char text1[50] = "You won!";
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
  char text1[50] = "You lost!";
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

