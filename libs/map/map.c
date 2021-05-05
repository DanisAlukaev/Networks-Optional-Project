#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "../wall/wall.h"

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
#define SPEED (300)
#define MAP ("libs/resources/background.jpg")

Wall *walls[6] = {NULL};

//
Map map_init(Map map, SDL_Window *window, SDL_Renderer *renderer) {
    //creating map texture
    map.map_texture = IMG_LoadTexture(renderer, MAP);

    //raising error if couldn't map create texture
    if (!map.map_texture) {
        printf("error creating map texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return map;
    }




    SDL_QueryTexture(map.map_texture, NULL, NULL, &map.map.w, &map.map.h);


    Wall wall1;
    wall1.x = 0;
    wall1.y = 100;
    wall1 = wall_init(wall1, window, renderer);
    map.walls[0] = wall1;

    Wall wall2;
    wall2.x = 350;
    wall2.y = 250;
    wall2 = wall_init(wall2, window, renderer);
    map.walls[1] = wall2;

    Wall wall3;
    wall3.x = 300;
    wall3.y = 450;
    wall3 = wall_init(wall3, window, renderer);
    map.walls[2] = wall3;

    Wall wall4;
    wall4.x = 100;
    wall4.y = 350;
    wall4 = wall_init(wall4, window, renderer);
    map.walls[3] = wall4;

    Wall wall5;
    wall5.x = 0;
    wall5.y = 220;
    wall5 = wall_init(wall5, window, renderer);
    map.walls[4] = wall5;

    Wall wall6;
    wall6.x = 370;
    wall6.y = 60;
    wall6 = wall_init(wall6, window, renderer);
    map.walls[5] = wall6;

    return map;
}

void render_map(SDL_Renderer *renderer, Map *map) {

    SDL_Texture *wallTexture;
    SDL_Surface *wall_s = IMG_Load("libs/resources/Wall4.jpg");
    if (!wall_s) {
        printf("Cannot find wall\n");
        return;
    }
    wallTexture = SDL_CreateTextureFromSurface(renderer, wall_s);
    SDL_FreeSurface(wall_s);
    //copying map to the renderer
    SDL_RenderCopy(renderer, map->map_texture, NULL, NULL);

    //rendering walls
    for (int i = 0; i < 6; i++) {
        SDL_RenderCopy(renderer, wallTexture, NULL, &map->walls[i].rec);
    }

}
