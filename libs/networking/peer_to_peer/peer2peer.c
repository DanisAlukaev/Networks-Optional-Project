#include "peer2peer.h"
#include "../client/client.h"
#include "../server/server.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>

void user_portal(struct PeerToPeer *peer_to_peer);

struct PeerToPeer peer_to_peer_constructor(int domain, int service, int protocol, u_long interface,
                                           char *known_hosts, int peers, int id, int **mapping,
                                           void *(*server_function)(void *arg),
                                           void *(*client_function)(void *arg)) {


    struct PeerToPeer peer_to_peer;
    peer_to_peer.domain = domain;
    peer_to_peer.service = service;
    peer_to_peer.protocol = protocol;
    peer_to_peer.interface = interface;
    peer_to_peer.known_hosts = known_hosts;
    peer_to_peer.mapping = mapping;
    peer_to_peer.peers = peers;
    peer_to_peer.id = id;


    int ports[28] = {1250, 1251, 1252, 1253, 1254, 1255, 1256, 1257, 1258, 1259,
                     1260, 1261, 1262, 1263, 1264, 1265, 1266, 1267, 1268, 1269,
                     1270, 1271, 1272, 1273, 1274, 1275, 1276, 1277};

    for (int i = 0; i < peers; i++) {
        if (i != id) {
            int port = ports[mapping[id][i]];
            peer_to_peer.servers[i] = server_constructor(domain, service, protocol, interface, port, 20);
        }
    }

    peer_to_peer.user_portal = user_portal;
    peer_to_peer.server_function = server_function;
    peer_to_peer.client_function = client_function;

    return peer_to_peer;
}

void user_portal(struct PeerToPeer *peer_to_peer) {
    pthread_t threads[peer_to_peer->peers-1];
    struct arg_struct args[peer_to_peer->peers-1];
    for (int i = 0; i < peer_to_peer->peers; i++) {
        if (i != peer_to_peer->id) {
            args[i].port_id = peer_to_peer->mapping[peer_to_peer->id][i];
            args[i].p2p = peer_to_peer;
            args[i].server_id = i;
            pthread_create(&threads[i], NULL, peer_to_peer->server_function, (void *) &args[i]);
        }
    }
    peer_to_peer->client_function(peer_to_peer);
}
