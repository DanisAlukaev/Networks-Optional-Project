#include "../libs/networking/client/client.h"
#include "../libs/networking/server/server.h"
#include "../libs/data_structures/lists/linked_list.h"

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>

#define PEERS 2

int main() {
    char request[255], server_address[INET_ADDRSTRLEN], client_address[INET_ADDRSTRLEN];
    struct Server announcement_server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 1249, PEERS);
    printf("Listening for connections.\n");
    inet_ntop(AF_INET, &announcement_server.address.sin_addr, server_address, INET_ADDRSTRLEN);
    printf("IP address of the announcement server: %s \n", server_address);
    struct LinkedList known_hosts = linked_list_constructor();
    struct sockaddr_in address;
    int address_length = sizeof(address);
    int connections_left, client;

    // process all requests from peers
    while (known_hosts.length <= PEERS) {
        client = accept(announcement_server.socket, (struct sockaddr *) &address, &address_length);
        memset(request, 0, 255);
        read(client, request, 255);
        struct sockaddr_in *pV4Addr = (struct sockaddr_in *) &address;
        struct in_addr ipAddr = pV4Addr->sin_addr;
        inet_ntop(AF_INET, &ipAddr, client_address, INET_ADDRSTRLEN);

        short found = 0;
        for (int i = 0; i < known_hosts.length && !found; i++) {
            if (strcmp(client_address, known_hosts.retrieve(&known_hosts, i)) == 0) {
                found = 1;
            }
        }
        if (!found) {
            known_hosts.append(&known_hosts, client_address, sizeof(client_address));
        }

        printf("Peer %s connected.\n", client_address);
        connections_left = PEERS - known_hosts.length;
        char response[255];
        if (connections_left != 0) {
            snprintf(response, sizeof response, "Connected successfully. \nWaiting for %d peer(s). \n",
                     connections_left);
        } else {
            snprintf(response, sizeof response, "Connected successfully. \nAll players were connected.\n");
        }
        send(client, response, strlen(response), 0);
        close(client);
    }

    // serialize known hosts
    char serialized_known_hosts[255] = "";
    for (int i = 0; i < known_hosts.length; i++) {
        char *host = known_hosts.retrieve(&known_hosts, i);
        strcat(serialized_known_hosts, host);
        strcat(serialized_known_hosts, " ");
    }

    // send list of known hosts to all peers
    connections_left = PEERS;
    while (connections_left > 0) {
        client = accept(announcement_server.socket, (struct sockaddr *) &address, &address_length);
        memset(request, 0, 255);
        read(client, request, 255);
        printf("Peer %s requested known hosts list.\n", client_address);
        connections_left--;
        send(client, serialized_known_hosts, strlen(serialized_known_hosts), 0);
        close(client);
    }
}

