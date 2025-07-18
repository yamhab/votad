#pragma once

#include "map.h"
#include "math.h"

#include <SDL3/SDL.h>

#define PLAYER_POS (Vector) { .x = 22.0, .y = 11.5 }
#define PLAYER_VEL (Vector) { .x = 0.0, .y = 0.0 }
#define PLAYER_DIR (Vector) { .x = -1.0, .y = 0.0 }
#define PLAYER_PLANE (Vector) { .x = 0.0, .y = -0.666 }
#define PLAYER_MOVE_SPEED 5.0
#define PLAYER_ROTATE_SPEED degToRad(135.0)

typedef struct Player {
    Vector pos, vel, dir, plane;
} Player;

void
playerRotate(Player *player, const bool *keys, double delta_time);

void
playerMove(Player *player, const bool *keys, double delta_time);

void
playerCollide(Player *player, const unsigned char map[MAP_WIDTH][MAP_HEIGHT]);
