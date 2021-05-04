#include "../libs/networking/peer_to_peer/peer2peer.h"
#include "../libs/networking/client/client.h"

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include<errno.h>

#define SERVERIP "10.91.49.128"
#define PEERS 2
char *known_hosts[PEERS];

char local_ip_address[100];

void get_local_ip_address(char *buffer, int count) {
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
    }
}

_Noreturn void *client_function(void *arg) {
    struct PeerToPeer *p2p = arg;
    while (1) {
        struct Client client = client_constructor(p2p->domain, p2p->service, p2p->protocol, p2p->port, p2p->interface);
        char request[255];
        memset(request, 0, 255);
        fgets(request, 255, stdin);
        for (int i = 0; i < PEERS; i++) {
            if (strcmp(local_ip_address, known_hosts[i]) != 0) {
                client.request(&client, known_hosts[i], request);
            }
        }
    }
}

int main() {
    int socket_cd;
    struct sockaddr_in server_address;
    char buffer[255] = {0};
    get_local_ip_address(local_ip_address, 100);
    printf("Current IP address: %s. \n", local_ip_address);

    // request pool
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
    read(socket_cd, buffer, 255);
    printf("%s\n", buffer);
    close(socket_cd);

    // wait for other peers
    int b;
    scanf("%d", &b);

    // request the list of known hosts

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
    for (i = 0; i < PEERS; i++)
        printf("%s\n", known_hosts[i]);

    struct PeerToPeer p2p = peer_to_peer_constructor(AF_INET, SOCK_STREAM, 0, 1248, INADDR_ANY, *known_hosts,
                                                     server_function, client_function);
    p2p.user_portal(&p2p);
}
