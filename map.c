#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "map.h"

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
#define SPEED (300)
#define MAP ("/home/makshe/CLionProjects/testing2/resources/background.jpg")


Map map_init(Map map, SDL_Window *window, SDL_Renderer *renderer){
    //creating map texture
    map.map_texture = IMG_LoadTexture(renderer, MAP);

    //raising error if couldn't map create texture
    if (!map.map_texture){
        printf("error creating map texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return map;
    }

    SDL_QueryTexture(map.map_texture, NULL, NULL, &map.map.w, &map.map.h);
    return map;
}
