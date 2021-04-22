#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include <time.h>
#include "sprite.h"
#include "../bullet/bullet.h"


#define MAX_BULLETS 1000
#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
#define SPEED (70)
#define SPRITESHEET ("/home/makshe/Downloads/S21-Networks-Optional-Project2/S21-Networks-Optional-Project/resources/playersprite.png")

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

    sprite.x_pos = (float) sprite.dest.x;
    sprite.y_pos = (float) sprite.dest.y;

    sprite.up = sprite.down = sprite.right = sprite.left = 0;
    sprite.direction = 0;


    return sprite;
}


void EventHandler(SDL_Event event, Sprite *sprite, int *close_requested, int shoot, Bullet *bullets[], long *start, long *end){


    while(SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_QUIT:
                *close_requested = 1;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_UP:
                        sprite->up = 1;
                        sprite->direction = 0;
                        break;
                    case SDL_SCANCODE_DOWN:
                        sprite->down = 1;
                        sprite->direction = 1;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        sprite->right = 1;
                        sprite->direction = 2;
                        break;
                    case SDL_SCANCODE_LEFT:
                        sprite->left = 1;
                        sprite->direction = 3;
                        break;
                    case SDL_SCANCODE_SPACE:
                        printf("start: %ld, end: %ld \n", *start, *end);
                        *end = clock();
                        if((*end - *start) >= 50000) {
                            *start = clock();
                            shoot = 1;
                            if (sprite->direction == 0) {
                                addBullet(bullets, sprite->x_pos + sprite->dest.w / 2 - 3, sprite->y_pos, 0, -3);
                            } else if (sprite->direction == 1) {
                                addBullet(bullets, sprite->x_pos + sprite->dest.w / 2 - 3, sprite->y_pos + 30, 0, 3);
                            } else if (sprite->direction == 2) {
                                addBullet(bullets, sprite->x_pos + 35, sprite->y_pos + sprite->dest.w / 2 + 1, 3, 0);
                            } else if (sprite->direction == 3) {
                                addBullet(bullets, sprite->x_pos + 10, sprite->y_pos + (sprite->dest.w) / 2 + 2, -3, 0);
                            }
                        }

                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_UP:
                        sprite->up = 0;
                        break;
                    case SDL_SCANCODE_DOWN:
                        sprite->down = 0;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        sprite->right = 0;
                        break;
                    case SDL_SCANCODE_LEFT:
                        sprite->left = 0;
                        break;
                    case SDL_SCANCODE_SPACE:
                        shoot=0;
                        break;
                }
                break;

        }
    }


    //determine the velocity
    int x_vel, y_vel;
    x_vel = y_vel = 0;
    if (sprite->up && !sprite->down && !sprite->right && !sprite->left) y_vel = -SPEED;
    if (sprite->down && !sprite->up && !sprite->right && !sprite->left) y_vel = SPEED;
    if (sprite->left && !sprite->right && !sprite->up && !sprite->down) x_vel = -SPEED;
    if (sprite->right && !sprite->left && !sprite->up && !sprite->down) x_vel = SPEED;

    //update positions:
    sprite->x_pos += (float)x_vel/45;
    sprite->y_pos += (float)y_vel/45;

    //collision detection:
    if(sprite->x_pos <= 0) sprite->x_pos = 0;
    if(sprite->y_pos <= 0) sprite->y_pos = 0;
    if(sprite->x_pos >= WINDOW_WIDTH - sprite->dest.w) sprite->x_pos = WINDOW_WIDTH - sprite->dest.w;
    if(sprite->y_pos >= WINDOW_HEIGHT - sprite->dest.h) sprite->y_pos = WINDOW_HEIGHT - sprite->dest.h;

    sprite->dest.y = (int) sprite->y_pos;
    sprite->dest.x = (int) sprite->x_pos;
    for(int i = 0; i < MAX_BULLETS; i++) if(bullets[i])
    {
        bullets[i]->x += bullets[i]->dx;
        bullets[i]->y += bullets[i]->dy;
        //simple coll. detection
        if(bullets[i]->x < -1000 || bullets[i]->x > 1000 || bullets[i]->y < -1000 || bullets[i]->y > 1000)
            //bullets[i]->x=10;
            remove_bullet(i, bullets);
    }

}



void RenderSprite(SDL_Renderer *renderer, Sprite *sprite){
    switch (sprite->direction){
        case 0:     //sprite is facing up
            SDL_RenderCopyEx(renderer, sprite->sprite_texture, &sprite->src, &sprite->dest, 0, NULL, 1);
            break;

        case 1:     //sprite is facing down
            SDL_RenderCopyEx(renderer, sprite->sprite_texture, &sprite->src, &sprite->dest, 180, NULL, 1);
            break;

        case 2:     //sprite is facing right
            SDL_RenderCopyEx(renderer, sprite->sprite_texture, &sprite->src, &sprite->dest, 90, NULL, 1);
            break;

        case 3:     //sprite is facing left
            SDL_RenderCopyEx(renderer, sprite->sprite_texture, &sprite->src, &sprite->dest, 270, NULL, 1);
            break;

    }

}




