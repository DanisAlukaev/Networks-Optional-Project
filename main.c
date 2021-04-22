#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include "sprite/sprite.h"
#include "map/map.h"
#include "bullet/bullet.h"
#include <SDL2/SDL_image.h>
#include <time.h>

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

#define MAX_BULLETS 1000

struct Bullet *bullets[MAX_BULLETS] = { NULL };

int main(void){
    int shoot=0;
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

    SDL_Texture *bulletTexture;

    SDL_Surface *bullet = IMG_Load("/home/makshe/Downloads/S21-Networks-Optional-Project2/S21-Networks-Optional-Project/resources/bullet.png");
    if(!bullet)
    {
        printf("Cannot find bullet\n");
        return 1;
    }

    bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);

    SDL_FreeSurface(bullet);



    Map map;
    map = map_init(map, window, renderer);

    Sprite sprite;
    sprite = sprite_init(sprite, window, renderer);

    //clear the window
    SDL_RenderClear(renderer);

    //keep track whether close button was pressed
    int close_requested = 0;

    long start, end;
    start = -1;
    end = clock();

    while(!close_requested){

        SDL_Event event;



        EventHandler(event, &sprite, &close_requested, shoot, bullets, &start, &end);

        //clearing the renderer
        SDL_RenderClear(renderer);

        //copying map to the renderer
        SDL_RenderCopy(renderer, map.map_texture, NULL, NULL);

        //rendering bullets
        for(int i = 0; i < MAX_BULLETS; i++) if(bullets[i])
        {
            SDL_Rect rect = { (int)bullets[i]->x,(int) bullets[i]->y, 8, 8 };
            SDL_RenderCopy(renderer, bulletTexture, NULL, &rect);
        }

        //rendering sprite
        RenderSprite(renderer, &sprite);

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
