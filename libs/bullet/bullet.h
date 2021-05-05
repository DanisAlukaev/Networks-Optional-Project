#ifndef TESTING2_BULLET_H
#define TESTING2_BULLET_H

typedef struct Bullet{
    float dx,dy;
    float x,y;
    int isShot;
}Bullet;

Bullet addBullet(struct Bullet *bullet, float x, float y, float dx, float dy);
Bullet remove_bullet(Bullet *bullet);

#endif //TESTING2_BULLET_H
