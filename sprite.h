#ifndef TESTING2_SPRITE_H
#define TESTING2_SPRITE_H


typedef struct Sprite{
    SDL_Texture* sprite_texture;
    int height, width, cur_frame;
    SDL_Rect src, dest;
}Sprite;


Sprite sprite_init(Sprite sprite, SDL_Window *window, SDL_Renderer *renderer);



#endif //TESTING2_SPRITE_H
