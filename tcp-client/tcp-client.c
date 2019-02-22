#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFSIZE 32

int main(int argc, char *argv[]) {
    // Check the command line parameters
    if (argc != 4) {
        fprintf(stderr, "Usage: tcp-client <server-ip> <port> <data>\n");
        return 1;
    }

    // Create the socket
    int sock;
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        fprintf(stderr, "Failed to create socket\n");
        return 1;
    }

    // Construct server address object
    struct sockaddr_in echoServer;
    echoServer.sin_family = AF_INET;
    echoServer.sin_addr.s_addr = inet_addr(argv[1]);
    echoServer.sin_port = htons(atoi(argv[2]));
    memset(&(echoServer.sin_zero), 0, sizeof(echoServer.sin_zero));

    // Establish connection
    if (connect(sock, 
        (struct sockaddr*) &echoServer,
        sizeof(echoServer)) < 0) {
        fprintf(stderr, "Failed to connect.\n");
        return 1;
    }

    // Send the data
    unsigned char dataLen = strlen(argv[3]);
    if (send(sock, argv[3], dataLen, 0) != dataLen) {
        fprintf(stderr, "Data check failure.\n");
        return 1;
    }

    // Receive the echo
    short received = 0;
    char buffer[BUFFSIZE];

    printf("Echo: \n");
    while (received < dataLen) {
        short packSize = 0;
        if ((packSize = recv(sock, buffer, BUFFSIZE-1, 0)) < 1) {
            fprintf(stderr, "Failed to receive byte.");
            return 1;
        }
        received += packSize;
    }

    // Print the echo
    buffer[received] = '\0';
    printf("%s\n", buffer);

    return 0;
}
