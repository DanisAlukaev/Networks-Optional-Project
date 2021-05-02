#include "../libs/networking/client/client.h"
#include "../libs/networking/server/server.h"
#include "../libs/data_structures/lists/linked_list.h"

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>

#define PEERS 1

// 192.168.1.73

int main() {
    printf("Listening for connections.\n");
    struct Server announcement_server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 1248, 20);
    char server_address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &announcement_server.address.sin_addr, server_address, INET_ADDRSTRLEN);
    printf("IP address of the announcement server: %s \n", server_address);
    struct LinkedList known_hosts;
    struct sockaddr_in address;
    int address_length = sizeof(address);
    while (known_hosts.length <= 1) {
        int client = accept(announcement_server.socket, (struct sockaddr *) &address, &address_length);
        char request[255];
        memset(request, 0, 255);
        read(client, request, 255);


        struct sockaddr_in *pV4Addr = (struct sockaddr_in *) &address;
        struct in_addr ipAddr = pV4Addr->sin_addr;
        char client_address[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &ipAddr, client_address, INET_ADDRSTRLEN);

        printf("Peer %s connected.\n", client_address);
        int connections_left = PEERS - known_hosts.length;
        char response[255] = "Connected successfully. \n";
        if (connections_left != 0) {
            snprintf(response, sizeof response, "%s%s%d \n", response, "Waiting for ", connections_left);
        } else {
            snprintf(response, sizeof response, "%s%s \n", response, "All players were connected.");
        }
        send(announcement_server.socket, response, strlen(response), 0);
        close(client);
        short found = 0;
        for (int i = 0; i < known_hosts.length && !found; i++) {
            if (strcmp(client_address, known_hosts.retrieve(&known_hosts, i)) == 0) {
                found = 1;
            }
        }
        if (!found) {
            known_hosts.append(&known_hosts, client_address, sizeof(client_address));
        }

        char serialized_known_hosts[255] = "";

        for (int i = 0; i < known_hosts.length; i++) {
            char *host = known_hosts.retrieve(&known_hosts, i);
            strcat(serialized_known_hosts, host);
        }

        connections_left = PEERS;

        while (connections_left > 0) {
            client = accept(announcement_server.socket, (struct sockaddr *) &address, &address_length);
            memset(request, 0, 255);
            read(client, request, 255);

            printf("Peer %s requested known hosts list.\n", client_address);
            connections_left--;
            send(announcement_server.socket, serialized_known_hosts, strlen(serialized_known_hosts), 0);
            close(client);
        }
    }
}

