#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include <time.h>
#include "sprite.h"
#include "../wall/wall.h"
#include "../bullet/bullet.h"


#define MAX_BULLETS 1000
#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
#define SPEED (70)

Sprite sprite_init(Sprite sprite, SDL_Window *window, SDL_Renderer *renderer) {

    int frame_width, frame_height;
    //creating texture

    sprite.alive=1;
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


    sprite.x_pos = (float) sprite.dest.x;
    sprite.y_pos = (float) sprite.dest.y;

    sprite.up = sprite.down = sprite.right = sprite.left = 0;
    sprite.direction = 0;

    sprite.message = (char*) malloc(20* sizeof(char));

    return sprite;
}

void avoid_wall(Wall *wall, Sprite *sprite){
    if(sprite->x_pos>=wall->x-sprite->dest.w && sprite->x_pos<=wall->x+(float)wall->rec.w && sprite->y_pos>=wall->y+(float)wall->rec.h && sprite->y_pos<=wall->y+(float)wall->rec.h+3) {
        sprite->y_pos = wall->y+(float)wall->rec.h+3;
    }
    if(sprite->x_pos>=wall->x-sprite->dest.w && sprite->x_pos<=wall->x+(float) wall->rec.w && sprite->y_pos>=wall->y-sprite->dest.h && sprite->y_pos<=wall->y) {
        sprite->y_pos = wall->y-sprite->dest.h;
    }
    if(sprite->x_pos>=(float) wall->x+wall->rec.w && sprite->x_pos<=(float) wall->x+wall->rec.w+5 && sprite->y_pos>=(float) wall->y-sprite->dest.h+2 && sprite->y_pos<=wall->y+(float) wall->rec.h) {
        sprite->x_pos = (float) wall->x+wall->rec.w+5;
    }
    if(sprite->x_pos>=(float) wall->x-sprite->dest.w - 3 && sprite->x_pos<=(float) wall->rec.x-sprite->dest.w && sprite->y_pos>=(float) wall->y-45 && sprite->y_pos<=wall->y+(float) wall->rec.h) {
        sprite->x_pos =(float) wall->x-sprite->dest.w-3;
    }
}

void EventHandler(SDL_Event event, Sprite *sprite, int *close_requested, Wall *walls[]){


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
                        if(sprite->alive) {
                            if (sprite->direction == 0 && !sprite->bullet.isShot) {
                                sprite->bullet=addBullet(&sprite->bullet, sprite->x_pos + sprite->dest.w / 2 - 3, sprite->y_pos, 0, -3);
                            }
                            else if (sprite->direction == 1 && !sprite->bullet.isShot) {
                                sprite->bullet=addBullet(&sprite->bullet, sprite->x_pos + sprite->dest.w / 2 - 3, sprite->y_pos + 50, 0,
                                          3);
                            }
                            else if (sprite->direction == 2 && !sprite->bullet.isShot) {
                                sprite->bullet=addBullet(&sprite->bullet, sprite->x_pos + 45, sprite->y_pos + sprite->dest.w / 2 + 1, 3,
                                          0);
                            }
                            else if (sprite->direction == 3 && !sprite->bullet.isShot) {
                                sprite->bullet=addBullet(&sprite->bullet, sprite->x_pos - 2, sprite->y_pos + (sprite->dest.w) / 2 + 2, -3,
                                          0);
                            }

                        }

                        break;

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

    SendPos(sprite);

    //collision detection:
    if(sprite->x_pos <= 0) sprite->x_pos = 0;
    if(sprite->y_pos <= 0) sprite->y_pos = 0;
    if(sprite->x_pos >= WINDOW_WIDTH - sprite->dest.w) sprite->x_pos = WINDOW_WIDTH - sprite->dest.w;
    if(sprite->y_pos >= WINDOW_HEIGHT - sprite->dest.h) sprite->y_pos = WINDOW_HEIGHT - sprite->dest.h;

    //walls
    for(int i = 0; i<6;i++) {
        avoid_wall(walls[i], sprite);

    }


    sprite->dest.y = (int) sprite->y_pos;
    sprite->dest.x = (int) sprite->x_pos;

    if(sprite->bullet.isShot) {
        sprite->bullet.x += sprite->bullet.dx;
        sprite->bullet.y += sprite->bullet.dy;
        if(sprite->bullet.x < 0 || sprite->bullet.x > 640 || sprite->bullet.y < 0 || sprite->bullet.y > 480)
            sprite->bullet=remove_bullet(&sprite->bullet);
        else if(sprite->bullet.x <= sprite->x_pos+sprite->dest.w && sprite->bullet.x >= sprite->x_pos && sprite->bullet.y <= sprite->y_pos+sprite->dest.h && sprite->bullet.y >= sprite->y_pos){
            sprite->alive=0;
            SDL_DestroyTexture(sprite->sprite_texture);
        }

        else {
            for (int j = 0; j < 6; j++) {
                if (sprite->bullet.x >= walls[j]->x-5 && sprite->bullet.x <= walls[j]->x + walls[j]->rec.w+1 &&
                    sprite->bullet.y >= walls[j]->y - 7 && sprite->bullet.y <= walls[j]->y + walls[j]->rec.h) {
                    sprite->bullet=remove_bullet(&sprite->bullet);
                    break;
                }

            }
        }
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


void SendPos(Sprite *sprite){
    int x, y, dir;
    x = sprite->dest.x;
    y = sprite->dest.y;
    dir = sprite->direction;
    sprintf(sprite->message, "%d %d %d \n", x, y, dir);
}


void RecvPos(char *message, Sprite *sprite){

    //char *message = "1324 1234 5"
    //char message[10]

    int nums[3];
    int len = strlen(message);
    char mes [len];

    for(int i = 0; i < len; i++){
        mes[i] = message[i];
    }

    char seps[] = " ";
    char* token;
    int var;
    int i = 0;

    token = strtok (mes, seps);
    while (token != NULL)
    {
        sscanf(token, "%d", &var);
        nums[i++] = var;

        token = strtok (NULL, seps);
    }

    sprite->dest.x = nums[0];
    sprite->dest.y = nums[1];
    sprite->direction = nums[2];

}

