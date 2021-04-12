#ifndef TESTING2_MAP_H
#define TESTING2_MAP_H

typedef struct Map{
    SDL_Texture* map_texture;
    SDL_Rect map;
}Map;

Map map_init(Map map, SDL_Window *window, SDL_Renderer *renderer);

#endif //TESTING2_MAP_H
