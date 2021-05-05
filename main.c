#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include "sprite/sprite.h"
#include "map/map.h"
#include "bullet/bullet.h"
#include <SDL2/SDL_image.h>
#include <time.h>
#include "wall/wall.h"
#define SPRITESHEET ("/home/talgat/Downloads/Networks3/S21-Networks-Optional-Project-visualization/resources/playersprite.png")
#define SPRITESHEET2 ("/home/talgat/Downloads/Networks3/S21-Networks-Optional-Project-visualization/resources/enemysprite.png")

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

#define MAX_BULLETS 1000


Wall *walls[6]={NULL};
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

    SDL_Texture *bulletTexture, *wallTexture;

    SDL_Surface *bullet = IMG_Load("/home/talgat/Downloads/Networks3/S21-Networks-Optional-Project-visualization/resources/bullet.png");

    if(!bullet)
    {
        printf("Cannot find bullet\n");
        return 1;
    }
    SDL_Surface *wall_s = IMG_Load("/home/talgat/Downloads/Networks3/S21-Networks-Optional-Project-visualization/resources/Wall4.jpg");
    if(!wall_s)
    {
        printf("Cannot find wall\n");
        return 1;
    }
    bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);
    wallTexture = SDL_CreateTextureFromSurface(renderer, wall_s);
    SDL_FreeSurface(bullet);
    SDL_FreeSurface(wall_s);

    Map map;
    map = map_init(map, window, renderer);

    Sprite sprite;
    sprite.sprite_texture = IMG_LoadTexture(renderer, SPRITESHEET);
    sprite.dest.x = (WINDOW_HEIGHT - sprite.dest.w)/2;
    sprite.dest.y = (WINDOW_HEIGHT - sprite.dest.h)/2;
    sprite = sprite_init(sprite, window, renderer);


    Wall wall1;
    wall1.x=0;
    wall1.y=100;
    wall1 = wall_init(wall1, window, renderer);
    walls[0]=&wall1;

    Wall wall2;
    wall2.x=350;
    wall2.y=250;
    wall2 = wall_init(wall2, window, renderer);
    walls[1]=&wall2;

    Wall wall3;
    wall3.x=300;
    wall3.y=450;
    wall3 = wall_init(wall3, window, renderer);
    walls[2]=&wall3;

    Wall wall4;
    wall4.x=100;
    wall4.y=350;
    wall4 = wall_init(wall4, window, renderer);
    walls[3]=&wall4;

    Wall wall5;
    wall5.x=0;
    wall5.y=220;
    wall5 = wall_init(wall5, window, renderer);
    walls[4]=&wall5;

    Wall wall6;
    wall6.x=370;
    wall6.y=60;
    wall6 = wall_init(wall6, window, renderer);
    walls[5]=&wall6;


    //clear the window
    SDL_RenderClear(renderer);

    //keep track whether close button was pressed
    int close_requested = 0;

    while(!close_requested){

        SDL_Event event;
        EventHandler(event, &sprite, &close_requested, walls);

        //clearing the renderer
        SDL_RenderClear(renderer);

        //copying map to the renderer
        SDL_RenderCopy(renderer, map.map_texture, NULL, NULL);

        //rendering walls
        for(int i = 0; i<6;i++) {
            SDL_RenderCopy(renderer, wallTexture, NULL, &walls[i]->rec);
        }

        //rendering bullets

        if(sprite.bullet.isShot) {
            printf("%d",34);
            SDL_Rect rect = {(int) sprite.bullet.x, (int) sprite.bullet.y, 12, 12};
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
