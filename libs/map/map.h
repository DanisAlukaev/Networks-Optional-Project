#ifndef TESTING2_MAP_H
#define TESTING2_MAP_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "../wall/wall.h"

typedef struct Map {
    SDL_Texture *map_texture;
    SDL_Rect map;
    Wall walls[6];
} Map;

Map map_init(Map map, SDL_Window *window, SDL_Renderer *renderer);

void render_map(SDL_Renderer *renderer, Map *map);

#endif //TESTING2_MAP_H
