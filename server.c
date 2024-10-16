/*
===========================================================================================
Name : ss34.c
Author : Dhruva Sharma
Description : Write a program to create a concurrent server
a. use fork
b. use pthread_reate
Output:

============================================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8080
#define BACKLOG 5

void handle_client(int new_socket) {
    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);
    printf("%s\n", buffer);
    send(new_socket, "recieved ack, connected", strlen("recieved ack, connected"), 0);
    close(new_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    

   address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        if (fork() == 0) {
            close(server_fd); // Child doesn't need the listening socket
            handle_client(new_socket);
            exit(0);
        }
        close(new_socket); // Parent doesn't need this specific client socket
    }

    return 0;
}
