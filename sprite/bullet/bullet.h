#ifndef TESTING2_BULLET_H
#define TESTING2_BULLET_H

typedef struct Bullet{
    SDL_Texture* bullet_texture;
    float dx,dy;
    float x,y;
    int width,height;
    SDL_Rect src, dest;
}Bullet;

void addBullet(struct Bullet *bullets[], float x, float y, float dx, float dy);
void remove_bullet(int i, struct Bullet *bullets[]);

#endif //TESTING2_BULLET_H
