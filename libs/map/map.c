#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "../wall/wall.h"

#define WINDOW_WIDTH (1280)
#define WINDOW_HEIGHT (720)
#define SPEED (130)
#define MAP ("libs/resources/background.jpg")

Wall *walls[20] = {NULL};

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
    wall1.x = 100;
    wall1.y = 55;
    wall1.rec.w = 100;
    wall1.rec.h = 250;
    wall1 = wall_init(wall1, window, renderer);
    map.walls[0] = wall1;

    Wall wall2;
    wall2.x = 298;
    wall2.y = 55;
    wall2.rec.w = 100;
    wall2.rec.h = 250;
    wall2 = wall_init(wall2, window, renderer);
    map.walls[1] = wall2;

    Wall wall3;
    wall3.x = 495;
    wall3.y = 55;
    wall3.rec.w = 100;
    wall3.rec.h = 193;
    wall3 = wall_init(wall3, window, renderer);
    map.walls[2] = wall3;

    Wall wall4;
    wall4.x = 594;
    wall4.y = 165;
    wall4.rec.w = 100;
    wall4.rec.h = 55;
    wall4 = wall_init(wall4, window, renderer);
    map.walls[3] = wall4;

    Wall wall5;
    wall5.x = 692;
    wall5.y = 55;
    wall5.rec.w = 100;
    wall5.rec.h = 193;
    wall5 = wall_init(wall5, window, renderer);
    map.walls[4] = wall5;

    Wall wall6;
    wall6.x = 888;
    wall6.y = 55;
    wall6.rec.w = 100;
    wall6.rec.h = 250;
    wall6 = wall_init(wall6, window, renderer);
    map.walls[5] = wall6;

    Wall wall7;
    wall7.x = 1085;
    wall7.y = 55;
    wall7.rec.w = 100;
    wall7.rec.h = 250;
    wall7 = wall_init(wall7, window, renderer);
    map.walls[6] = wall7;

    Wall wall8;
    wall8.x = 0;
    wall8.y = 358;
    wall8.rec.w = 100;
    wall8.rec.h = 55;
    wall8 = wall_init(wall8, window, renderer);
    map.walls[7] = wall8;

    Wall wall9;
    wall9.x = 200;
    wall9.y = 358;
    wall9.rec.w = 195;
    wall9.rec.h = 55;
    wall9 = wall_init(wall9, window, renderer);
    map.walls[8] = wall9;

    Wall wall10;
    wall10.x = 495;
    wall10.y = 304;
    wall10.rec.w = 100;
    wall10.rec.h = 55;
    wall10 = wall_init(wall10, window, renderer);
    map.walls[9] = wall10;



    Wall wall11;
    wall11.x = 695;
    wall11.y = 304;
    wall11.rec.w = 100;
    wall11.rec.h = 55;
    wall11 = wall_init(wall11, window, renderer);
    map.walls[10] = wall11;

    Wall wall12;
    wall12.x = 888;
    wall12.y = 358;
    wall12.rec.w = 195;
    wall12.rec.h = 55;
    wall12 = wall_init(wall12, window, renderer);
    map.walls[11] = wall12;

    Wall wall13;
    wall13.x = 1184;
    wall13.y = 358;
    wall13.rec.w = 100;
    wall13.rec.h = 55;
    wall13 = wall_init(wall13, window, renderer);
    map.walls[12] = wall13;

    Wall wall14;
    wall14.x = 100;
    wall14.y = 470;
    wall14.rec.w = 100;
    wall14.rec.h = 193;
    wall14 = wall_init(wall14, window, renderer);
    map.walls[13] = wall14;

    Wall wall15;
    wall15.x = 298;
    wall15.y = 470;
    wall15.rec.w = 100;
    wall15.rec.h = 193;
    wall15 = wall_init(wall15, window, renderer);
    map.walls[14] = wall15;

    Wall wall16;
    wall16.x = 495;
    wall16.y = 414;
    wall16.rec.w = 100;
    wall16.rec.h = 166;
    wall16 = wall_init(wall16, window, renderer);
    map.walls[15] = wall16;

    Wall wall17;
    wall17.x = 594;
    wall17.y = 442;
    wall17.rec.w = 100;
    wall17.rec.h = 55;
    wall17 = wall_init(wall17, window, renderer);
    map.walls[16] = wall17;

    Wall wall18;
    wall18.x = 692;
    wall18.y = 414;
    wall18.rec.w = 100;
    wall18.rec.h = 166;
    wall18 = wall_init(wall18, window, renderer);
    map.walls[17] = wall18;

    Wall wall19;
    wall19.x = 888;
    wall19.y = 470;
    wall19.rec.w = 100;
    wall19.rec.h = 193;
    wall19 = wall_init(wall19, window, renderer);
    map.walls[18] = wall19;

    Wall wall20;
    wall20.x = 1085;
    wall20.y = 470;
    wall20.rec.w = 100;
    wall20.rec.h = 193;
    wall20 = wall_init(wall20, window, renderer);
    map.walls[19] = wall20;

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
    for (int i = 0; i < 20; i++) {
        SDL_RenderCopy(renderer, wallTexture, NULL, &map->walls[i].rec);
    }

}
