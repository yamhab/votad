#pragma once

#include "math.h"
#include "player.h"

#include <SDL3/SDL.h>

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240

#define TEXTURES_LENGTH 8
#define TEXTURE_SIZE 64

typedef struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *textures[TEXTURES_LENGTH];
    unsigned long ticks;
    double delta_time;
    Player player;
} Game;

SDL_AppResult
gameInit(Game **game);

SDL_AppResult
texturesInit(Game *game);

SDL_AppResult
gameIterate(Game *game);

SDL_AppResult
gameRender(Game *game);

SDL_AppResult
mapRender(Game *game);

SDL_AppResult
columnRender(Game *game, double wall_dist, bool side, Vector ray_dir, unsigned char map_tile, size_t x);

SDL_AppResult
gameHandleEvent(Game *game, SDL_Event *event);

void
gameQuit(Game *game, SDL_AppResult result);
