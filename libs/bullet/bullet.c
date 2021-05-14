#include <SDL2/SDL_timer.h>
#include "bullet.h"

#define WINDOW_WIDTH (1280)
#define WINDOW_HEIGHT (720)
#define SPEED (300)

/**
 * function that creates a bullet near the tank when tanks shoots and allocates memory for the bullet
 */
Bullet addBullet(Bullet *bullet,float x, float y, float dx, float dy)
{
    bullet = malloc(sizeof(struct Bullet));
    bullet->x = x;
    bullet->y = y;
    bullet->dx = dx;
    bullet->dy = dy;
    bullet->isShot=1;
    return *bullet;
}

/**
 * function that removes a bullet when necessary by moving it outside the map and setting isShot=0
 */
Bullet remove_bullet(Bullet *bullet)
{
    bullet->x = -100;
    bullet->y = -100;
    bullet->dx = 0;
    bullet->dy = 0;
    bullet->isShot=0;


    return *bullet;
}