#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "sprite.h"
#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
#define SPEED (300)
#define SPRITESHEET ("/home/makshe/CLionProjects/testing2/resources/spritesheet.png")

Sprite sprite_init(Sprite sprite, SDL_Window *window, SDL_Renderer *renderer) {

    int frame_width, frame_height;
    //creating texture
    sprite.sprite_texture = IMG_LoadTexture(renderer, SPRITESHEET);

    //raising error if couldn't create texture
    if (!sprite.sprite_texture) {
        printf("error creating sprite texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return sprite;
    }

    SDL_QueryTexture(sprite.sprite_texture, NULL, NULL, &frame_width, &frame_height);

    frame_width /= 4;
    sprite.width = frame_width;
    sprite.height = frame_height;

    sprite.src.x = 0;
    sprite.src.y = 0;
    sprite.src.w = sprite.width;
    sprite.src.h = sprite.height;

    sprite.dest.x = (WINDOW_HEIGHT - frame_width)/2;
    sprite.dest.y = (WINDOW_HEIGHT - frame_height)/2;
    sprite.dest.w = sprite.src.w/2;
    sprite.dest.h = sprite.src.h/2;
    sprite.cur_frame = sprite.width;

    return sprite;
}





void EventHandler(SDL_Event event, Sprite sprite, int up, int down ){




}








