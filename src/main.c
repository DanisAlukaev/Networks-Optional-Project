#include "../libs/networking/peer_to_peer/peer2peer.h"
#include "../libs/networking/client/client.h"
#include "../libs/sprite/sprite.h"
#include <signal.h>

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

#define SERVERIP "10.91.54.190"
#define PEERS 3
#define ENEMYSPRITE ("libs/resources/enemysprite.png")
#define PLAYERSPRITE ("libs/resources/playersprite.png")
#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

char *known_hosts[PEERS];
Sprite tanks[PEERS];
SDL_Window *window;

Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
SDL_Renderer *renderer;
Map map;
int my_id = -1;
SDL_Texture *bulletTexture;
SDL_Surface *bullet;

char local_ip_address[100];

//structure that is used to store x,y coordinates
struct Coordinate {
    int x;
    int y;
};

// сreating spawn_points for tanks
struct Coordinate spawn_points[8] = {{.x = 0, .y=240},
                                     {.x = 0, .y=430},
                                     {.x = 599, .y=430},
                                     {.x = 599, .y=0},
                                     {.x = 0, .y=0},
                                     {.x = 0, .y=143},
                                     {.x = 440, .y=200},
                                     {.x = 407, .y=332}};

int ports[28] = {1250, 1251, 1252, 1253, 1254, 1255, 1256, 1257, 1258, 1259,
                 1260, 1261, 1262, 1263, 1264, 1265, 1266, 1267, 1268, 1269,
                 1270, 1271, 1272, 1273, 1274, 1275, 1276, 1277};

int **mapping_ports_ids_peers_ids;

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
    render_map(renderer, &map);
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
    char *hello = "JOIN_LOBBY";
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
        char *request = "GET_NUMBER_PLAYERS";
        send(socket_cd, request, strlen(request), 0);
        read(socket_cd, buffer, 255);
        printf("%s\n", buffer);
        close(socket_cd);
        if (strcmp(buffer, "Done") == 0)
            break;
    }
    printf("Synchronization... \n");
    sleep(5);
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
    int i = 0;
    char *p = strtok(buffer, " ");
    while (p != NULL) {
        known_hosts[i] = p;
        char *tempo = (char *) malloc(20 * sizeof(char));
        strcpy(tempo, p);
        known_hosts[i] = tempo;
        i++;
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
    int port = args->port_id;
    int server_id = args->server_id;

    printf("Server running on port %d.\n", ports[port]);
    struct sockaddr_in address;
    int address_length = sizeof(address);

    while (1) {
        int client = accept(p2p->servers[server_id].socket, (struct sockaddr *) &address,
                            (socklen_t *) &address_length);
        char request[255];
        memset(request, 0, 255);
        read(client, request, 255);
//        printf("Received %s\n", request);

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

/**
 * Client routine for the peer.
 *
 * Primarily, the method initializes set of sprites.
 * Until the window is closed, it handles user's action, then updates data of sprite accordingly.
 * Data describing the sprite, i.e. its position, rotation and position of bullet is serialized.
 * Serialized data is sent to each peer via correspondent socket.
 *
 * @param arg - instance of class PeerToPeer.
 */
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
        char *request = tanks[my_id].message;

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

/**
 * Method used to map indices of known hosts and ports.
 *
 * For instance, the result for 4 PEERS will be
 * -1   0   1   2
 *  0  -1   3   4
 *  1   3  -1   5
 *  2   4   5  -1
 *
 *  Accordingly, for socket creation between peer with index 0 and peer with index 1 will be used port with index 0.
 */
void init_mapping_peers_ports() {
    mapping_ports_ids_peers_ids = (int **) malloc(PEERS * sizeof(int *));
    for (int i = 0; i < PEERS; i++) {
        mapping_ports_ids_peers_ids[i] = (int *) malloc(PEERS * sizeof(int));
    }
    int incremented_port = 0;
    for (int i = 0; i < PEERS; i++) {
        for (int j = i; j < PEERS; j++) {
            if (i == j) {
                mapping_ports_ids_peers_ids[i][j] = -1;
                continue;
            }
            mapping_ports_ids_peers_ids[i][j] = incremented_port;
            mapping_ports_ids_peers_ids[j][i] = incremented_port;
            incremented_port++;
        }
    }
}


int main() {
    // disable errors for broken pipe occurring when socket is busy/sigaction(SIGPIPE, &(struct sigaction) {SIG_IGN}, NULL);
    sigaction(SIGPIPE, &(struct sigaction) {SIG_IGN}, NULL);
    // join lobby and fill in list of known hosts
    join_waiting_room();
    // initialize canvas and sprites
    init_variables_visualization();
    // fill in table with ids mapping peers addresses and ports for socket creation
    init_mapping_peers_ports();


    struct PeerToPeer p2p = peer_to_peer_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, *known_hosts, PEERS, my_id,
                                                     mapping_ports_ids_peers_ids, server_function, client_function);
    p2p.user_portal(&p2p);
}