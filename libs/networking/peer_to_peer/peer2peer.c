#include "peer2peer.h"
#include "../client/client.h"
#include "../server/server.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>

void user_portal(struct PeerToPeer *peer_to_peer);

struct PeerToPeer peer_to_peer_constructor(int domain, int service, int protocol, int port, u_long interface,
                                           char *known_hosts, void *(*server_function)(void *arg),
                                           void *(*client_function)(void *arg)) {
    struct PeerToPeer peer_to_peer;
    peer_to_peer.domain = domain;
    peer_to_peer.service = service;
    peer_to_peer.protocol = protocol;
    peer_to_peer.port = port;
    peer_to_peer.interface = interface;
    peer_to_peer.known_hosts = known_hosts;
    peer_to_peer.server = server_constructor(domain, service, protocol, interface, port, 20);


    peer_to_peer.user_portal = user_portal;
    peer_to_peer.server_function = server_function;
    peer_to_peer.client_function = client_function;

    return peer_to_peer;
}

void user_portal(struct PeerToPeer *peer_to_peer) {
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, peer_to_peer->server_function, peer_to_peer);
    peer_to_peer->client_function(peer_to_peer);
}