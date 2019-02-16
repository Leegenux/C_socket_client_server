#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFSIZE 32
#define MAXPENDING 5

int main(int argc, char *argv[]) {
    //
    if (argc != 2) {
        fprintf(stderr, "Usage: tcp-server <port>");
        return 1;
    }

    // Create main socket
    int serverSock;

    // Create TCP socket
    if ((serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        fprintf(stderr, "Failed to create socket\n");
        return 1;
    }

    // Set main socket address
    struct sockaddr_in echoServer;
    echoServer.sin_family = AF_INET;
    echoServer.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServer.sin_port = htons(atoi(argv[1]));
    memset(&(echoServer.sin_zero), 0, sizeof(echoServer.sin_zero));

    // Bind the main socket
    if (bind(serverSock,
        (struct sockaddr*) &echoServer,
        sizeof(echoServer)) < 0) {
        fprintf(stderr, "Failed to bind.\n");
        return 1;
    }
    
    // Listen to the main socket
    if (listen(serverSock, MAXPENDING) < 0) {
        fprintf(stderr, "Failed to listen on server socket.\n");
        return 1;
    }

    // Handle the requests
    int clientSock;
    struct sockaddr_in echoClient;
    unsigned int clientLen = sizeof(echoClient);

    while (1) {
        if ((clientSock = accept(serverSock,
                            (struct sockaddr*) &echoClient,
                            &clientLen)) < 0) {
            fprintf(stderr, "Failed to accept the connection.\n");
            return 1;
        }
        printf("Client connected: %s\n", inet_ntoa(echoClient.sin_addr));

        // Do the echo
        char buffer[BUFFSIZE];
        int received = -1;
        if ((received = recv(clientSock, buffer, BUFFSIZE, 0)) < 0) {
            fprintf(stderr, "Failed to receive data.\n");
            return 1;
        }

        while (received > 0) {
            if (send(clientSock, buffer, received, 0) != received) {
                fprintf(stderr, "Failed to echo back.\n");
                return 1;
            }
            if ((received = recv(clientSock, buffer, BUFFSIZE, 0)) < 0) {
                fprintf(stderr, "Failed to receive data.\n");
            }
        }
        close(clientSock);
    }

    return 0;
}