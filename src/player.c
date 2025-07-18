#include "math.h"
#include "player.h"

#include <SDL3/SDL.h>

void
playerRotate(Player *player, const bool *keys, double delta_time) {
    if (keys[SDL_SCANCODE_LEFT]) {
        player->dir = vectorRotate(player->dir, -PLAYER_ROTATE_SPEED * delta_time);
        player->plane = vectorRotate(player->plane, -PLAYER_ROTATE_SPEED * delta_time);
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        player->dir = vectorRotate(player->dir, PLAYER_ROTATE_SPEED * delta_time);
        player->plane = vectorRotate(player->plane, PLAYER_ROTATE_SPEED * delta_time);
    }
}

void
playerMove(Player *player, const bool *keys, double delta_time) {
    player->vel = PLAYER_VEL;

    if (keys[SDL_SCANCODE_W])
        player->vel = vectorTranslate(player->vel, player->dir);
    if (keys[SDL_SCANCODE_S])
        player->vel = vectorTranslate(player->vel, vectorScale(player->dir, -1.0));
    if (keys[SDL_SCANCODE_A])
        player->vel = vectorTranslate(player->vel, vectorScale(player->plane, -1.0));
    if (keys[SDL_SCANCODE_D])
        player->vel = vectorTranslate(player->vel, player->plane);

    if (vectorMagnitude(player->vel))
        player->vel = vectorScale(vectorNormalize(player->vel), PLAYER_MOVE_SPEED * delta_time);
}

void
playerCollide(Player *player, const unsigned char map[MAP_WIDTH][MAP_HEIGHT]) {
    Vector pos = player->pos;
    if (!map[(size_t) (pos.x + player->vel.x)][(size_t) pos.y]) {
        player->pos.x = pos.x + player->vel.x;
    }
    if (!map[(size_t) pos.x][(size_t) (pos.y + player->vel.y)]) {
        player->pos.y = pos.y + player->vel.y;
    }
}
