cmake_minimum_required(VERSION 3.17)
project(testing2 C)

set(CMAKE_C_STANDARD 11)

add_executable(testing2 src/main.c libs/sprite/sprite.c libs/sprite/sprite.h libs/map/map.c libs/map/map.h libs/bullet/bullet.c libs/bullet/bullet.h libs/wall/wall.c libs/wall/wall.h)

target_link_libraries(${PROJECT_NAME} SDL2_image-2.0 SDL2main SDL2)
