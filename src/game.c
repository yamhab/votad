#include "game.h"
#include "map.h"
#include "math.h"

#include <SDL3/SDL.h>

#include <stdio.h>
#include <stdlib.h>

SDL_AppResult
gameInit(Game **game) {
    if ((*game = malloc(sizeof(Game))) == NULL)
        return SDL_APP_FAILURE;

    if (!SDL_SetAppMetadata("Vengeance of the Abyssal Depths", "1.0.0",
                            "com.yamhab.vengeance-of-the-abyssal-depths"))
        return SDL_APP_FAILURE;

    if (!SDL_Init(SDL_INIT_VIDEO))
        return SDL_APP_FAILURE;

    if (!SDL_CreateWindowAndRenderer("Vengeance of the Abyssal Depths", WINDOW_WIDTH, WINDOW_HEIGHT,
                                     SDL_WINDOW_FULLSCREEN, &(*game)->window, &(*game)->renderer))
        return SDL_APP_FAILURE;

    SDL_HideCursor();
    SDL_SetRenderLogicalPresentation((*game)->renderer, WINDOW_WIDTH, WINDOW_HEIGHT,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderVSync((*game)->renderer, -1);

    SDL_AppResult init_textures = texturesInit(*game);
    if (init_textures)
        return init_textures;

    (*game)->ticks = 0;

    (*game)->player.pos = PLAYER_POS;
    (*game)->player.vel = PLAYER_VEL;
    (*game)->player.dir = PLAYER_DIR;
    (*game)->player.plane = PLAYER_PLANE;

    return SDL_APP_CONTINUE;
}

SDL_AppResult
texturesInit(Game *game) {
    if (!(game->textures[0] = SDL_LoadBMP("textures/eagle.bmp")))
        return SDL_APP_FAILURE;
    if (!(game->textures[1] = SDL_LoadBMP("textures/redbrick.bmp")))
        return SDL_APP_FAILURE;
    if (!(game->textures[2] = SDL_LoadBMP("textures/purplestone.bmp")))
        return SDL_APP_FAILURE;
    if (!(game->textures[3] = SDL_LoadBMP("textures/greystone.bmp")))
        return SDL_APP_FAILURE;
    if (!(game->textures[4] = SDL_LoadBMP("textures/bluestone.bmp")))
        return SDL_APP_FAILURE;
    if (!(game->textures[5] = SDL_LoadBMP("textures/mossy.bmp")))
        return SDL_APP_FAILURE;
    if (!(game->textures[6] = SDL_LoadBMP("textures/wood.bmp")))
        return SDL_APP_FAILURE;
    if (!(game->textures[7] = SDL_LoadBMP("textures/colorstone.bmp")))
        return SDL_APP_FAILURE;

    return SDL_APP_CONTINUE;
}

SDL_AppResult
gameIterate(Game *game) {
    unsigned long ticks = SDL_GetTicksNS();
    game->delta_time = (ticks - game->ticks) / 1000000000.0;
    game->ticks = ticks;

    int numkeys;
    const bool *keys = SDL_GetKeyboardState(&numkeys);
    playerRotate(&game->player, keys, game->delta_time);
    playerMove(&game->player, keys, game->delta_time);
    playerCollide(&game->player, MAP);

    return gameRender(game);
}

SDL_AppResult
gameRender(Game *game) {
    SDL_Renderer *renderer = game->renderer;

    if (!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255))
        return SDL_APP_FAILURE;
    if (!SDL_RenderClear(renderer))
        return SDL_APP_FAILURE;

    SDL_AppResult map = mapRender(game);
    if (map)
        return map;

    if (!SDL_RenderPresent(renderer))
        return SDL_APP_FAILURE;

    return SDL_APP_CONTINUE;
}

SDL_AppResult
mapRender(Game *game) {
    Player player = game->player;

    for (size_t x = 0; x < WINDOW_WIDTH; ++x) {
        double camera_column = 2.0 * x / (double) WINDOW_HEIGHT - 1.0;
        Vector ray_dir = vectorTranslate(player.dir, vectorScale(player.plane, camera_column));

        Vector side_dist;
        Vector delta_dist = (Vector) {
            .x = (ray_dir.x == 0.0) ? 1e30 : fabs(1.0 / ray_dir.x),
            .y = (ray_dir.y == 0.0) ? 1e30 :  fabs(1.0 / ray_dir.y)
        };

        int step_x;
        int step_y;

        int map_x = player.pos.x;
        int map_y = player.pos.y;

        if (ray_dir.x < 0.0) {
            step_x = -1;
            side_dist.x = delta_dist.x * (player.pos.x - map_x);
        } else {
            step_x = 1;
            side_dist.x = delta_dist.x * (map_x + 1.0 - player.pos.x);
        }
        if (ray_dir.y < 0.0) {
            step_y = -1;
            side_dist.y = delta_dist.y * (player.pos.y - map_y);
        } else {
            step_y = 1;
            side_dist.y = delta_dist.y * (map_y + 1.0 - player.pos.y);
        }

        bool hit = false;
        bool side;
        while (!hit) {
            if (side_dist.x < side_dist.y) {
                side_dist.x += delta_dist.x;
                map_x += step_x;
                side = false;
            } else {
                side_dist.y += delta_dist.y;
                map_y += step_y;
                side = true;
            }
            if (MAP[map_x][map_y] > 0) hit = true;
        }

        double wall_dist;
        if (!side)
            wall_dist = side_dist.x - delta_dist.x;
        else
            wall_dist = side_dist.y - delta_dist.y;

        SDL_AppResult column = columnRender(game, wall_dist, side, ray_dir, MAP[map_x][map_y], x);
        if (column)
            return column;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult
columnRender(Game *game, double wall_dist, bool side, Vector ray_dir, unsigned char map_tile,
             size_t x) {
    SDL_Renderer *renderer = game->renderer;

    int line_height = WINDOW_HEIGHT / wall_dist;
    int draw_start = -line_height / 2 + WINDOW_HEIGHT / 2;
    int draw_end = line_height / 2 + WINDOW_HEIGHT / 2;

    if (draw_start < 0) draw_start = 0;
    if (draw_end >= WINDOW_HEIGHT) draw_end = WINDOW_HEIGHT;

    double wall_x;
    if (!side)
        wall_x = game->player.pos.y + wall_dist * ray_dir.y;
    else
        wall_x = game->player.pos.x + wall_dist * ray_dir.x;
    wall_x -= floor((wall_x));

    int texture_x = (int) (wall_x * (double) TEXTURE_SIZE);
    if (!side && ray_dir.x > 0) texture_x = TEXTURE_SIZE - texture_x - 1;
    if (side == 1 && ray_dir.y < 0) texture_x = TEXTURE_SIZE - texture_x - 1;

    double step = 1.0 * TEXTURE_SIZE / line_height;
    double texture_pos = (draw_start - WINDOW_HEIGHT / 2.0 + line_height / 2.0) * step;

    if (!SDL_SetRenderDrawColor(renderer, 0x55, 0x33, 0x11, 0xFF))
        return SDL_APP_FAILURE;
    if (!SDL_RenderLine(renderer, x, 0, x, draw_start))
        return SDL_APP_FAILURE;

    for (size_t y = draw_start; y < draw_end; y++) {
        int texture_y = (int) texture_pos & (TEXTURE_SIZE - 1);
        texture_pos += step;

        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
        SDL_ReadSurfacePixel(game->textures[map_tile - 1], TEXTURE_SIZE - texture_x - 1, texture_y, &r, &g, &b, &a);
        if (side) {
            r /= 2;
            g /= 2;
            b /= 2;
        }
        r = (short) r - wall_dist * 5 < 0 ? 0 : r - wall_dist * 5;
        g = (short) g - wall_dist * 5 < 0 ? 0 : g - wall_dist * 5;
        b = (short) b - wall_dist * 5 < 0 ? 0 : b - wall_dist * 5;

        if (!SDL_SetRenderDrawColor(renderer, r, g, b, a))
            return SDL_APP_FAILURE;
        if (!SDL_RenderPoint(renderer, x, y))
            return SDL_APP_FAILURE;
    }

    if (!SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF))
        return SDL_APP_FAILURE;
    if (!SDL_RenderLine(renderer, x, draw_end, x, WINDOW_HEIGHT))
        return SDL_APP_FAILURE;

    return SDL_APP_CONTINUE;
}

SDL_AppResult
gameHandleEvent(Game *game, SDL_Event *event) {
    switch (event->type) {
        case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

void
gameQuit(Game *game, SDL_AppResult result) {
    SDL_DestroyWindow(game->window);
    SDL_DestroyRenderer(game->renderer);
    for (size_t i = 0; i < TEXTURES_LENGTH; ++i)
        SDL_DestroySurface(game->textures[i]);
    free(game);

    if (result == SDL_APP_FAILURE)
        fprintf(stderr, "%s\n", SDL_GetError());
}
