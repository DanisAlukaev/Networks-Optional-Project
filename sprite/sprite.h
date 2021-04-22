#ifndef TESTING2_SPRITE_H
#define TESTING2_SPRITE_H
#include "../map/map.h"
#include "../bullet/bullet.h"


typedef struct Sprite{
    SDL_Texture* sprite_texture;
    float x_pos, y_pos;
    int direction;
    int up, down, right, left;
    SDL_Rect src, dest;
}Sprite;


Sprite sprite_init(Sprite sprite, SDL_Window *window, SDL_Renderer *renderer);

void EventHandler(SDL_Event event, Sprite *sprite, int *close_requested, int shoot, Bullet *bullets[], long *start, long *end);


void RenderSprite(SDL_Renderer *renderer, Sprite *sprite);

#endif //TESTING2_SPRITE_H
