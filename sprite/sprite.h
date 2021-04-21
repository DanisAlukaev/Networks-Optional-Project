#ifndef TESTING2_SPRITE_H
#define TESTING2_SPRITE_H


#include "../map/map.h"

typedef struct Sprite{
    SDL_Texture* sprite_texture;
    float x_pos, y_pos, x_vel, y_vel;
    int direction[4];
    int *up, *down, *right, *left;
    SDL_Rect src, dest;
}Sprite;


Sprite sprite_init(Sprite sprite, SDL_Window *window, SDL_Renderer *renderer);

void EventHandler(SDL_Window *window, SDL_Renderer *renderer, SDL_Event event, Sprite *sprite, Map map, int *close_requested);

void MoveSprite(SDL_Window *window, SDL_Renderer *renderer, Sprite *sprite);


#endif //TESTING2_SPRITE_H
