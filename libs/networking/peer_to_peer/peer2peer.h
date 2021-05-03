#ifndef peer2peer_h
#define peer2peer_h

#include "../server/server.h"
#include "../../data_structures/lists/linked_list.h"

struct PeerToPeer {
    struct Server server;
    char *known_hosts;

    int domain;
    int service;
    int protocol;
    int port;
    u_long interface;

    void (*user_portal)(struct PeerToPeer *peer_to_peer);

    void *(*server_function)(void *arg);

    void *(*client_function)(void *arg);
};

struct PeerToPeer
peer_to_peer_constructor(int domain, int service, int protocol, int port, u_long interface, char *known_hosts,
                         void *(*server_function)(void *arg), void *(*client_function)(void *arg));

#endif