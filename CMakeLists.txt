cmake_minimum_required(VERSION 3.17)
project(Source C)

set(CMAKE_C_STANDARD 99)
add_executable(${PROJECT_NAME} src/main.c)

add_subdirectory(./libs/data_structures/lists)
target_link_libraries(${PROJECT_NAME} LinkedLists)

add_subdirectory(./libs/networking/peer_to_peer)
target_link_libraries(${PROJECT_NAME} PeerToPeer)

add_executable(testing2 src/main.c libs/sprite/sprite.c libs/sprite/sprite.h libs/map/map.c libs/map/map.h libs/bullet/bullet.c libs/bullet/bullet.h libs/wall/wall.c libs/wall/wall.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} pthread)

target_link_libraries(${PROJECT_NAME} SDL2_image-2.0 SDL2main SDL2)
