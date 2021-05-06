#include "../libs/networking/peer_to_peer/peer2peer.h"
#include "../libs/networking/client/client.h"
#include "../libs/sprite/sprite.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include<errno.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <signal.h>

#define SERVERIP "10.91.54.190"
#define PEERS 3
#define ENEMYSPRITE ("libs/resources/enemysprite.png")
#define PLAYERSPRITE ("libs/resources/playersprite.png")
#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

char *known_hosts[PEERS];
Sprite tanks[PEERS];
short got_the_packet[PEERS] = {0};
SDL_Window *window;

Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
SDL_Renderer *renderer;
Map map;
int my_id = -1;
SDL_Texture *bulletTexture;
SDL_Surface *bullet;

char local_ip_address[100];

int ports[28] = {1248, 1249, 1250, 1251, 1252, 1253, 1254, 1255, 1256, 1257,
                 1258, 1259, 1260, 1261, 1262, 1263, 1264, 1265, 1266, 1267,
                 1268, 1269, 1270, 1271, 1272, 1273, 1274, 1275};

int mapping_ports_ids_peers_ids[PEERS][PEERS];


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

    int code = connect(sock, (const struct sockaddr *) &serv, sizeof(serv));

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


    // initialize map
    map = map_init(map, window, renderer);
    render_map(renderer, &map);
    bullet = IMG_Load("libs/resources/bullet.png");
    if (!bullet) {
        printf("Cannot find bullet\n");
    }
    bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);
    SDL_FreeSurface(bullet);
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
    char *hello = "JOIN_POOL";
    send(socket_cd, hello, strlen(hello), 0);
    read(socket_cd, buffer, 255);
    printf("%s\n", buffer);
    close(socket_cd);

    // waiting for other peers
    while (1) {
        sleep(1);
        memset(buffer, 0, 255);
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
        char *request = "GET_NUMBER_PLAYERS";
        send(socket_cd, request, strlen(request), 0);
        read(socket_cd, buffer, 255);
        printf("%s\n", buffer);
        close(socket_cd);
        if (strcmp(buffer, "Done") == 0)
            break;
    }
    printf("Synchronization... \n");
    sleep(8);
    printf("Request for known hosts... \n");
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

    char *request_known_hosts = "GET_KNOWN_HOSTS";
    send(socket_cd, request_known_hosts, strlen(request_known_hosts), 0);
    memset(buffer, 0, 255);
    read(socket_cd, buffer, 255);
    close(socket_cd);

    // form list of known hosts
    int idx = 0;
    char *p = strtok(buffer, " ");
    while (p != NULL) {
        known_hosts[idx] = p;
        char *tempo = (char *) malloc(20 * sizeof(char));
        strcpy(tempo, p);
        known_hosts[idx] = tempo;
        idx++;
        p = strtok(NULL, " ");
    }


    for (int i = 0; i < PEERS; i++)
        if (strcmp(local_ip_address, known_hosts[i]) == 0)
            my_id = i;
}

void init_sprite(int id) {
    // initialized my coordinates
    Sprite sprite;

    if (id == my_id) {
        sprite.sprite_texture = IMG_LoadTexture(renderer, PLAYERSPRITE);
    } else {
        sprite.sprite_texture = IMG_LoadTexture(renderer, ENEMYSPRITE);
    }
    sprite.dest.x = spawn_points[id].x;
    sprite.dest.y = spawn_points[id].y;
    sprite = sprite_init(sprite, window, renderer);

    tanks[id] = sprite;
}

_Noreturn void *server_function(void *arg) {
    struct arg_struct *args = (struct arg_struct *) arg;
    struct PeerToPeer *p2p = args->p2p;
    int port_id = args->port_id;
    struct sockaddr_in address;
    int address_length = sizeof(address);

    printf("Server on port %d is running.\n", ports[port_id]);

    while (1) {
        int client = accept(p2p->servers[port_id].socket, (struct sockaddr *) &address, (socklen_t *) &address_length);
        char request[255];
        memset(request, 0, 255);
        read(client, request, 255);

        // get string representation of client's ip
        struct sockaddr_in *pV4Addr = (struct sockaddr_in *) &address;
        struct in_addr ipAddr = pV4Addr->sin_addr;
        char client_address[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &ipAddr, client_address, INET_ADDRSTRLEN);

        // determine index of the tank
        int peer_idx = -1;
        for (int i = 0; i < PEERS; i++) {
            if (strcmp(client_address, known_hosts[i]) == 0) {
                peer_idx = i;
                break;
            }
        }
        // update data of the sprite
        RecvPos(request, &tanks[peer_idx]);
        close(client);
    }
}

_Noreturn void *client_function(void *arg) {
    struct PeerToPeer *p2p = arg;

    // initialize set of sprites
    for (int i = 0; i < PEERS; i++)
        init_sprite(i);

    // while the window is open
    int close_requested = 0;
    while (!close_requested) {
        // check for event
        SDL_Event event;
        EventHandler(event, &tanks[my_id], &close_requested, map.walls);

        // update message field of the sprite
        SendPos(&tanks[my_id]);
        char *request = (char *) malloc(20 * sizeof(char));
        request = tanks[my_id].message;
        for (int i = 0; i < PEERS; i++) {
            // for all peers except the current
            // connect to the socket
            if (i != my_id) {
                struct Client client = client_constructor(p2p->domain, p2p->service, p2p->protocol,
                                                          ports[mapping_ports_ids_peers_ids[my_id][i]], p2p->interface);
                client.request(&client, known_hosts[i], request);
            }
        }

        // render the map, sprite, and bullets
        render_map(renderer, &map);
        for (int i = 0; i < PEERS; i++) {
            RenderSprite(renderer, &tanks[i]);
            if (tanks[i].bullet.isShot) {
                SDL_Rect rect = {(int) tanks[i].bullet.x, (int) tanks[i].bullet.y, 12, 12};
                SDL_RenderCopy(renderer, bulletTexture, NULL, &rect);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
        usleep(50 * 1000);
    }

    // clean up resources before exiting
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(map.map_texture);
    for (int i = 0; i < PEERS; i++) {
        SDL_DestroyTexture(tanks[i].sprite_texture);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void init_mapping_peers_ports() {
    int incremented_port = 0;
    for (int i = 0; i < PEERS; i++) {
        for (int j = i; j < PEERS; j++) {
            if (i == j) {
                mapping_ports_ids_peers_ids[i][j] = -1;
            } else {
                mapping_ports_ids_peers_ids[i][j] = incremented_port;
                mapping_ports_ids_peers_ids[j][i] = incremented_port;
                incremented_port++;
            }
        }
    }
    for (int i = 0; i < PEERS; i++) {
        for (int j = 0; j < PEERS; j++) {
            printf("%d ", mapping_ports_ids_peers_ids[i][j]);
        }
        printf("\n");
    }
}


int main() {
    sigaction(SIGPIPE, &(struct sigaction) {SIG_IGN}, NULL);
    join_waiting_room();
    init_variables_visualization();
    init_mapping_peers_ports();
    struct PeerToPeer p2p = peer_to_peer_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, PEERS,
                                                     mapping_ports_ids_peers_ids, my_id,
                                                     server_function, client_function);
    p2p.user_portal(&p2p);
}