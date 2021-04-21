#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include "sprite/sprite.h"
#include "map/map.h"

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)


int main(void){

    int window_width, window_height;
    window_width = WINDOW_WIDTH;
    window_height = WINDOW_HEIGHT;

    // attempt to initialize graphics and timer system
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Tanks 667",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          window_width, window_height, 0);
    if (!window)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }


    // creating renderer
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, render_flags);
    if(!renderer){
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    Map map;
    map = map_init(map, window, renderer);

    Sprite sprite;
    sprite = sprite_init(sprite, window, renderer);

    //clear the window
    SDL_RenderClear(renderer);

    //keep track whether close button was pressed
    int close_requested = 0;

    while(!close_requested){
        SDL_Event event;

        EventHandler(event, &sprite, &close_requested);


        //clearing the renderer
        SDL_RenderClear(renderer);

        //copying map to the renderer
        SDL_RenderCopy(renderer, map.map_texture, NULL, NULL);


        //rotating sprite according to its direction
        switch (sprite.direction){
            case 0:     //sprite is facing up
                SDL_RenderCopyEx(renderer, sprite.sprite_texture, &sprite.src, &sprite.dest, 0, NULL, 1);
                break;

            case 1:     //sprite is facing down
                SDL_RenderCopyEx(renderer, sprite.sprite_texture, &sprite.src, &sprite.dest, 180, NULL, 1);
                break;

            case 2:     //sprite is facing right
                SDL_RenderCopyEx(renderer, sprite.sprite_texture, &sprite.src, &sprite.dest, 90, NULL, 1);
                break;

            case 3:     //sprite is facing left
                SDL_RenderCopyEx(renderer, sprite.sprite_texture, &sprite.src, &sprite.dest, 270, NULL, 1);
                break;

        }

        //SDL_RenderCopy(renderer, sprite.sprite_texture, &sprite.src, &sprite.dest);
        SDL_RenderPresent(renderer);

        SDL_Delay(0);
    }

    // clean up resources before exiting
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(map.map_texture);
    SDL_DestroyTexture(sprite.sprite_texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
