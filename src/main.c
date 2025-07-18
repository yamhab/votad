#include "game.h"

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_timer.h>

SDL_AppResult
SDL_AppInit(void **game, int argc, char *argv[]) {
    return gameInit((Game **) game);
}

SDL_AppResult
SDL_AppIterate(void *game) {
    return gameIterate((Game *) game);
}

SDL_AppResult
SDL_AppEvent(void *game, SDL_Event *event) {
    return gameHandleEvent((Game *) game, event);
}

void
SDL_AppQuit(void *game, SDL_AppResult result) {
    gameQuit((Game *) game, result);
}
