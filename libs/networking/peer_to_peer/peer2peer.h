#ifndef peer2peer_h
#define peer2peer_h

#include "../server/server.h"
#include "../../data_structures/lists/linked_list.h"


struct arg_struct {
    int port_id;
    struct PeerToPeer *p2p;
};

struct PeerToPeer {
    struct Server servers[8];
    int peers;
    int domain;
    int service;
    int protocol;
    int **mapping;
    int id;
    u_long interface;


    void (*user_portal)(struct PeerToPeer *peer_to_peer);

    void *(*server_function)(void *arg);

    void *(*client_function)(void *arg);
};

struct PeerToPeer
peer_to_peer_constructor(int domain, int service, int protocol, u_long interface, int peers, int mapping[peers][peers],
                         int my_id, void *(*server_function)(void *arg), void *(*client_function)(void *arg));

#endif