#ifndef TESTING2_WALL_H
#define TESTING2_WALL_H

typedef struct Wall{
    float x, y;
    int direction;
    SDL_Rect rec;

}Wall;

Wall wall_init(Wall wall, SDL_Window *window, SDL_Renderer *renderer);

#endif //TESTING2_WALL_H