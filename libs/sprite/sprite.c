#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <unistd.h>
#include "sprite.h"

#define WINDOW_WIDTH (1280)
#define WINDOW_HEIGHT (720)
#define SPEED (300)
#define PEERS 3

/**
 * function that initializes the tank by setting its dimension and initial position
 * as well as its direction and isShot to zero
 */
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
    sprite.dest.w = 60;
    sprite.dest.h = 60;


    sprite.x_pos = (float) sprite.dest.x;
    sprite.y_pos = (float) sprite.dest.y;

    sprite.up = sprite.down = sprite.right = sprite.left = 0;
    sprite.direction = 0;
    sprite.bullet.isShot = 0;
    remove_bullet(&sprite.bullet);
    sprite.message = (char*) malloc(26* sizeof(char));

    return sprite;
}

/**
 * function for collision detection between sprite and wall
 * All four cases are considered (when tank moves from down, up, left, and right)
 * tank will stay in the same position if it wants to move into the wall
 */
void avoid_wall(Wall *wall, Sprite *sprite){
    if(sprite->x_pos>wall->x-sprite->dest.w+5 && sprite->x_pos<wall->x+(float)wall->rec.w-5 && sprite->y_pos<=wall->y+(float)wall->rec.h+20 && sprite->y_pos>wall->y+(float)wall->rec.h+5) {
        sprite->y_pos = wall->y+(float)wall->rec.h+20;
    }
    if(sprite->x_pos>wall->x-sprite->dest.w+5 && sprite->x_pos<wall->x+(float)wall->rec.w-5 && sprite->y_pos>=wall->y-sprite->dest.h+10 && sprite->y_pos<wall->y) {
        sprite->y_pos = wall->y-sprite->dest.h+10;
    }
    if(sprite->x_pos>=(float) wall->x+wall->rec.w-10 && sprite->x_pos<=(float) wall->x+wall->rec.w+10 && sprite->y_pos>=(float) wall->y-sprite->dest.h+15 && sprite->y_pos<=wall->y+(float) wall->rec.h+5) {
        sprite->x_pos = (float) wall->x+wall->rec.w+10;
    }
    if(sprite->x_pos>=(float) wall->x-sprite->dest.w-5 && sprite->x_pos<=(float) wall->x-sprite->dest.w+20 && sprite->y_pos>=(float) wall->y-sprite->dest.h+15 && sprite->y_pos<=wall->y+(float) wall->rec.h+5) {
        sprite->x_pos =(float) wall->x-sprite->dest.w-5;
    }
}

/**
 * function that handles a particular event and moves a sprite to the direction pressed by the button
 * Also it creates a bullet if tank has shot and moves the bullet to the corresponding direction
 * until collision with wall or another sprite
 * If tank was hit by the bullet, both the tank and bullet will disappear
 */
void EventHandler(SDL_Event event, Sprite *sprite, int *close_requested, Map map, Sprite *tanks){

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
                                sprite->bullet=addBullet(&sprite->bullet, sprite->x_pos + sprite->dest.w / 2 - 3, sprite->y_pos-8, 0, -20);
                            }
                            else if (sprite->direction == 1 && !sprite->bullet.isShot) {
                                sprite->bullet=addBullet(&sprite->bullet, sprite->x_pos + sprite->dest.w / 2 - 3, sprite->y_pos + sprite->dest.h+8, 0,
                                                         20);
                            }
                            else if (sprite->direction == 2 && !sprite->bullet.isShot) {
                                sprite->bullet=addBullet(&sprite->bullet, sprite->x_pos + 60 + 7, sprite->y_pos + sprite->dest.w / 2 - 3, 20,
                                                         0);
                            }
                            else if (sprite->direction == 3 && !sprite->bullet.isShot) {
                                sprite->bullet=addBullet(&sprite->bullet, sprite->x_pos - 7, sprite->y_pos + (sprite->dest.w) / 2 - 3, -20,
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


    //collision detection:
    if(sprite->x_pos <= 0) sprite->x_pos = 0;
    if(sprite->y_pos <= 0) sprite->y_pos = 0;
    if(sprite->x_pos >= WINDOW_WIDTH - sprite->dest.w) sprite->x_pos = WINDOW_WIDTH - sprite->dest.w;
    if(sprite->y_pos >= WINDOW_HEIGHT - sprite->dest.h) sprite->y_pos = WINDOW_HEIGHT - sprite->dest.h;

    //walls
    for(int i = 0; i<20;i++) {
        avoid_wall(&map.walls[i], sprite);
    }

    sprite->dest.y = (int) sprite->y_pos;
    sprite->dest.x = (int) sprite->x_pos;
    for(int i=0;i<PEERS;i++){
        for(int j=0;j<PEERS;j++) {
            if (tanks[i].bullet.x <= tanks[j].x_pos +tanks[j].dest.w && tanks[i].bullet.x >= tanks[j].x_pos &&
                tanks[i].bullet.y <= tanks[j].y_pos + tanks[j].dest.h && tanks[i].bullet.y >= tanks[j].y_pos) {
                tanks[j].alive = 0;
                tanks[i].bullet = remove_bullet(&tanks[i].bullet);
            }
        }
    }
    if(sprite->bullet.isShot) {
        sprite->bullet.x += sprite->bullet.dx;
        sprite->bullet.y += sprite->bullet.dy;
        if(sprite->bullet.x < 0 || sprite->bullet.x > 1280 || sprite->bullet.y < 0 || sprite->bullet.y > 720)
            sprite->bullet=remove_bullet(&sprite->bullet);


        else {
            for (int j = 0; j < 20; j++) {
                if (sprite->bullet.x >= map.walls[j].x - 5 && sprite->bullet.x <= map.walls[j].rec.x + map.walls[j].rec.w &&
                    sprite->bullet.y >= map.walls[j].rec.y - 7 && sprite->bullet.y <= map.walls[j].rec.y + map.walls[j].rec.h) {
                    sprite->bullet=remove_bullet(&sprite->bullet);
                    break;
                }

            }
        }
    }

}

/**
 * function that rotates  the sprite's image by some angle (90, 180, 270 degrees)
 * according to the direction
 */
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

/**
 * function that collects the sprites information into the message that will be sent to other peers
 */
void SendPos(Sprite *sprite){
    int x, y, dir, bullet_x, bullet_y, alive;
    x = sprite->dest.x;
    y = sprite->dest.y;
    dir = sprite->direction;
    alive = sprite->alive;
    bullet_x = (int) sprite->bullet.x;
    bullet_y = (int) sprite->bullet.y;
    sprintf(sprite->message, "%d %d %d %d %d %d\n", x, y, dir, bullet_x, bullet_y, alive);
}

/**
 * function that initializes the sprite according to the message that is received from other peers
 * this is used to render them on the window
 */
void RecvPos(char *message, Sprite *sprite){
    int nums[6];
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
    sprite->bullet.x = (float) nums[3];
    sprite->bullet.y = (float) nums[4];
    sprite->alive = nums[5];
}

