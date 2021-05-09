#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "wall.h"
#include "../sprite/sprite.h"
#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
#define SPEED (300)

/* method that is used to create wall on corresponding position with given weight and height
 * then we return the wall
 */
Wall wall_init(Wall wall, SDL_Window *window, SDL_Renderer *renderer){
    //creating map texture
    int frame_width, frame_height;
    wall.rec.x=(int) wall.x;
    wall.rec.y=(int) wall.y;
    return wall;
}


