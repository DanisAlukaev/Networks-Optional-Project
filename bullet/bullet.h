#ifndef TESTING2_BULLET_H
#define TESTING2_BULLET_H

typedef struct Bullet{
    float dx,dy;
    float x,y;
}Bullet;

void addBullet(struct Bullet *bullets[], float x, float y, float dx, float dy);
void remove_bullet(int i, struct Bullet *bullets[]);

#endif //TESTING2_BULLET_H
