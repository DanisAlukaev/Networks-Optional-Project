#include "server.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<errno.h>

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

    int err = connect(sock, (const struct sockaddr *) &serv, sizeof(serv));

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr *) &name, &namelen);

    const char *p = inet_ntop(AF_INET, &name.sin_addr, buffer, 100);
    if (p == NULL)  {
        printf("Error %d occurred: %s \n", errno, strerror(errno));
    }
    close(sock);
}

struct Server server_constructor(int domain, int service, int protocol, u_long interface, int port, int backlog) {
    struct Server server;
    // Define the basic parameters of the server.
    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;

    // Compose address of the server.
    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    char local_ip_address[100];
    get_local_ip_address(local_ip_address, 100);
    server.address.sin_addr.s_addr = inet_addr(local_ip_address);

    // Create a socket for the server.
    server.socket = socket(domain, service, protocol);
    // Confirm the connection was successful.
    if (server.socket == 0) {
        perror("Failed to connect a socket...\n");
        exit(1);
    }
    // Attempt to bind the socket to the network.
    if ((bind(server.socket, (struct sockaddr *) &server.address, sizeof(server.address))) < 0) {
        perror("Failed to bind a socket...\n");
        exit(1);
    }
    // Start listening on the network.
    if ((listen(server.socket, server.backlog)) < 0) {
        perror("Failed to start listening...\n");
        exit(1);
    }
    return server;
}