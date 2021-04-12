cmake_minimum_required(VERSION 3.17)
project(testing2 C)

set(CMAKE_C_STANDARD 11)

add_executable(testing2 main.c sprite.c sprite.h map.c map.h)

target_link_libraries(${PROJECT_NAME} SDL2_image-2.0 SDL2main SDL2)
