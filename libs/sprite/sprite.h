#ifndef TESTING2_SPRITE_H
#define TESTING2_SPRITE_H

#include <SDL2/SDL.h>
#include "../map/map.h"
#include "../bullet/bullet.h"
#include "../wall/wall.h"

typedef struct Sprite{
    SDL_Texture* sprite_texture;
    float x_pos, y_pos;
    int direction;
    Bullet bullet;
    int up, down, right, left;
    SDL_Rect src, dest;
    char* message;
    int alive;
}Sprite;


Sprite sprite_init(Sprite sprite, SDL_Window *window, SDL_Renderer *renderer);

void EventHandler(SDL_Event event, Sprite *sprite, int *close_requested, Wall walls[]);

void RenderSprite(SDL_Renderer *renderer, Sprite *sprite);

void avoid_wall(Wall *wall, Sprite *sprite);

void RecvPos(char *message, Sprite *sprite);

void SendPos(Sprite *sprite);

#endif //TESTING2_SPRITE_H
