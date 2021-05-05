#include "../libs/networking/peer_to_peer/peer2peer.h"
#include "../libs/networking/client/client.h"
#include "../libs/sprite/sprite.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include<errno.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#define SERVERIP "10.91.49.128"
#define PEERS 2
#define SPRITESHEET ("libs/resources/playersprite.png")
#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

char *known_hosts[PEERS];
Sprite *tanks[PEERS];
short got_the_packet[PEERS] = {0};
SDL_Window *window;

Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
SDL_Renderer *renderer;
Map map;
int my_id = -1;

char local_ip_address[100];

//structure that is used to store x,y coordinates
struct Coordinate {
    int x;
    int y;
};

// сreating spawn_points for tanks
struct Coordinate spawn_points[10] = {{.x = 0, .y=240},
                                      {.x = 0, .y=430},
                                      {.x = 599, .y=430},
                                      {.x = 599, .y=0},
                                      {.x = 0, .y=0},
                                      {.x = 0, .y=143},
                                      {.x = 440, .y=200},
                                      {.x = 407, .y=332}};

/* Method used to get local ip address of the peer.
 * Establishes connection with Google DNS server.
 *
 * buffer - char array allocated for the ip address.
 */
void get_local_ip_address(char *buffer) {
    const char *google_dns_server = "8.8.8.8";
    int dns_port = 53;

    struct sockaddr_in serv;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket error");
    }
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(google_dns_server);
    serv.sin_port = htons(dns_port);

    connect(sock, (const struct sockaddr *) &serv, sizeof(serv));

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    getsockname(sock, (struct sockaddr *) &name, &namelen);

    const char *p = inet_ntop(AF_INET, &name.sin_addr, buffer, 100);
    if (p == NULL) {
        printf("Error %d occurred: %s \n", errno, strerror(errno));
    }
    close(sock);
}

/* Method used to initialize variables used for visualization.
 * Variables include window, renderer
 *
 */
int init_variables_visualization() {
    // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    // initialize window
    window = SDL_CreateWindow("Tanks 667",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // initialize renderer
    renderer = SDL_CreateRenderer(window, -1, render_flags);
    if (!renderer) {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // TODO: move info about the bullets to the sprite
    SDL_Texture *bulletTexture;
    SDL_Surface *bullet = IMG_Load("libs/resources/bullet.png");
    if (!bullet) {
        printf("Cannot find bullet\n");
        return 1;
    }
    bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);
    SDL_FreeSurface(bullet);

    // initialize map
    map = map_init(map, window, renderer);
}

void join_waiting_room() {
    int socket_cd;
    struct sockaddr_in server_address;
    char buffer[255] = {0};
    get_local_ip_address(local_ip_address);
    printf("Current IP address: %s. \n", local_ip_address);

    // request pool
    if ((socket_cd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(1249);

    if (inet_pton(AF_INET, SERVERIP, &server_address.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }

    if (connect(socket_cd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("\nConnection Failed \n");
        return;
    }
    char *hello = "JOINPUL";
    send(socket_cd, hello, strlen(hello), 0);
    read(socket_cd, buffer, 255);
    printf("%s\n", buffer);
    close(socket_cd);

    // waiting for other peers
    while (1) {
        sleep(1);
        int socket_cd;
        struct sockaddr_in server_address;
        char buffer[255] = {0};
        get_local_ip_address(local_ip_address);
        printf("Current IP address: %s. \n", local_ip_address);

        // request pool
        if ((socket_cd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            continue;
        }

        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(1249);

        if (inet_pton(AF_INET, SERVERIP, &server_address.sin_addr) <= 0) {
            printf("\nInvalid address/ Address not supported \n");
            continue;
        }

        if (connect(socket_cd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
            printf("\nConnection Failed \n");
            continue;
        }
        char *request = "GETNUMPLAYERS";
        send(socket_cd, request, strlen(request), 0);
        read(socket_cd, buffer, 255);
        printf("%s\n", buffer);
        close(socket_cd);
        if (strcmp(buffer, "Done") == 0)
            break;
    }
    printf("Synchronization...");
    sleep(8);
    printf("Request for known hosts...");
    // request the list of known hosts

    if ((socket_cd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(1249);

    if (inet_pton(AF_INET, SERVERIP, &server_address.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }

    if (connect(socket_cd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("\nConnection Failed \n");
        return;
    }

    char *request_known_hosts = "RQSTKNWNHST";
    send(socket_cd, request_known_hosts, strlen(request_known_hosts), 0);
    memset(buffer, 0, 255);
    read(socket_cd, buffer, 255);
    close(socket_cd);

    // form list of known hosts
    int i = 0;
    char *p = strtok(buffer, " ");
    while (p != NULL) {
        known_hosts[i++] = p;
        p = strtok(NULL, " ");
    }

    for (int i = 0; i < PEERS; i++)
        if (strcmp(local_ip_address, known_hosts[i]) == 0)
            my_id = i;
}

void init_my_sprite() {
    // initialized my coordinates
    Sprite sprite;
    sprite.sprite_texture = IMG_LoadTexture(renderer, SPRITESHEET);
    sprite.dest.x = spawn_points[my_id].x;
    sprite.dest.y = spawn_points[my_id].y;
    sprite = sprite_init(sprite, window, renderer);
    tanks[my_id] = &sprite;
}

_Noreturn void *server_function(void *arg) {
    printf("Server running.\n");
    init_my_sprite();
    struct PeerToPeer *p2p = (struct PeerToPeer *) arg;
    struct sockaddr *address = (struct sockaddr *) &p2p->server.address;
    socklen_t address_length = (socklen_t) sizeof(p2p->server.address);

    while (1) {
        int client = accept(p2p->server.socket, address, &address_length);
        char request[255];
        memset(request, 0, 255);
        read(client, request, 255);
        char *client_address = inet_ntoa(p2p->server.address.sin_addr);
        printf("\t\t\t%s says: %s\n", client_address, request);

        int peer_idx = -1;
        for (int i = 0; i < PEERS; i++) {
            if (strcmp(client_address, known_hosts[i]) != 0) {
                peer_idx = i;
                break;
            }
        }
        if (got_the_packet[peer_idx] == 0) {
            got_the_packet[peer_idx] = 1;
            Sprite new_enemy;
            tanks[peer_idx] = &new_enemy;
        }
        RecvPos(request, tanks[peer_idx]);
        close(client);
    }
}

_Noreturn void *client_function(void *arg) {
    struct PeerToPeer *p2p = arg;

    SDL_RenderClear(renderer);
    int close_requested = 0;

    while (!close_requested) {
        SDL_Event event;
        EventHandler(event, tanks[my_id], &close_requested, map.walls);
        //SDL_RenderCopy(renderer, sprite.sprite_texture, &sprite.src, &sprite.dest);
        SDL_RenderPresent(renderer);

        SDL_Delay(0);

        // while update
        struct Client client = client_constructor(p2p->domain, p2p->service, p2p->protocol, p2p->port, p2p->interface);
        char *request = (char *) malloc(20 * sizeof(char));

        SendPos(tanks[my_id]);

        request = tanks[my_id]->message;

        for (int i = 0; i < PEERS; i++) {
            if (strcmp(local_ip_address, known_hosts[i]) != 0) {
                client.request(&client, known_hosts[i], request);
            }
        }

    }

    // clean up resources before exiting
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(map.map_texture);
    for (int i = 0; i < PEERS; i++) {
        SDL_DestroyTexture(tanks[i]->sprite_texture);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}


int main() {
    join_waiting_room();
    init_variables_visualization();
    struct PeerToPeer p2p = peer_to_peer_constructor(AF_INET, SOCK_STREAM, 0, 1248, INADDR_ANY, *known_hosts,
                                                     server_function, client_function);
    p2p.user_portal(&p2p);
}