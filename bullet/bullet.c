#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include "bullet.h"

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
#define SPEED (300)
#define MAX_BULLETS 1000

void addBullet(Bullet *bullets[],float x, float y, float dx, float dy)
{
    int found = -1;
    for(int i = 0; i < MAX_BULLETS; i++)
    {
        if(bullets[i] == NULL)
        {
            found = i;
            break;
        }
    }

    if(found >= 0)
    {
        int i = found;
        bullets[i] = malloc(sizeof(struct Bullet));
        bullets[i]->x = x;
        bullets[i]->y = y;
        bullets[i]->dx = dx;
        bullets[i]->dy = dy;
    }
}


void remove_bullet(int i, struct Bullet *bullets[1000])
{
    if(bullets[i])
    {
        free(bullets[i]);
        bullets[i] = NULL;
    }
//    return *bullets;
}