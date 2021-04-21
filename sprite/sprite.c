#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "sprite.h"
#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
#define SPEED (300)
#define SPRITESHEET ("/home/makshe/PullFolder/S21-Networks-Optional-Project/resources/playersprite.png")

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

    //getting dimensions of the texture
    SDL_QueryTexture(sprite.sprite_texture, NULL, NULL, &frame_width, &frame_height);

    //setting dimensions of the source rect
    sprite.src.x = 0;
    sprite.src.y = 0;
    sprite.src.w = frame_width;
    sprite.src.h =  frame_height;

    //setting dimensions of the destination rect
    sprite.dest.w = sprite.src.w/2;
    sprite.dest.h = sprite.src.h/2;
    sprite.dest.x = (WINDOW_HEIGHT - sprite.dest.w)/2;
    sprite.dest.y = (WINDOW_HEIGHT - sprite.dest.h)/2;

    return sprite;
}









