#ifndef client_h
#define client_h

#include <sys/socket.h>
#include <netinet/in.h>

struct Client {
    // Implementation of client structure.

    // Network socket.
    int socket;
    //
    int domain;
    int service;
    int protocol;
    int port;
    u_long interface;

    // Method that allow to send the request to specified server.
    char *(*request)(struct Client *client, char *server_ip, char *request);
};

// Constructor for the client structure.
struct Client client_constructor(int domain, int service, int protocol, int port, u_long interface);

#endif