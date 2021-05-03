#include "../libs/networking/peer_to_peer/peer2peer.h"
#include "../libs/networking/client/client.h"

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>

#define SERVERIP "10.91.49.128"
#define PEERS 2
char *known_hosts[PEERS];


_Noreturn void *server_function(void *arg) {
    printf("Server running.\n");
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
        close(client);
        short found = 0;
        for (int i = 0; i < p2p->known_hosts.length && !found; i++) {
            if (strcmp(client_address, p2p->known_hosts.retrieve(&p2p->known_hosts, i)) == 0) {
                found = 1;
            }
        }
        if (!found) {
            p2p->known_hosts.append(&p2p->known_hosts, client_address, sizeof(client_address));
        }
    }
}

_Noreturn void *client_function(void *arg) {
    struct PeerToPeer *p2p = arg;
    while (1) {
        struct Client client = client_constructor(p2p->domain, p2p->service, p2p->protocol, p2p->port, p2p->interface);
        char request[255];
        memset(request, 0, 255);
        fgets(request, 255, stdin);
        for (int i = 0; i < p2p->known_hosts.length; i++) {
            client.request(&client, p2p->known_hosts.retrieve(&p2p->known_hosts, i), request);
        }
    }
}

int main() {
    int socket_cd = 0, valread;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};

    if ((socket_cd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(1249);

    if (inet_pton(AF_INET, SERVERIP, &server_address.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(socket_cd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    char *hello = "JOINPUL";
    send(socket_cd, hello, strlen(hello), 0);
    valread = read(socket_cd, buffer, 1024);
    printf("%s\n", buffer);
    close(socket_cd);

    clock_t start_time = clock();
    while (start_time + 10000 < clock());

    if ((socket_cd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(1249);

    if (inet_pton(AF_INET, SERVERIP, &server_address.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(socket_cd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    char *request_known_hosts = "RQSTKNWNHST";
    send(socket_cd, request_known_hosts, strlen(request_known_hosts), 0);
    valread = read(socket_cd, buffer, 1024);
    printf("%s\n", buffer);
    close(socket_cd);


    int i = 0;
    char *p = strtok(buffer, " ");

    while (p != NULL) {
        known_hosts[i++] = p;
        p = strtok(NULL, " ");
    }

    for (i = 0; i < PEERS; ++i)
        printf("%s\n", known_hosts[i]);


    struct PeerToPeer p2p = peer_to_peer_constructor(AF_INET, SOCK_STREAM, 0, 1248, INADDR_ANY, server_function,
                                                     client_function);
    p2p.user_portal(&p2p);
}