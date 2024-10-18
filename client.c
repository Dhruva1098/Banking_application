#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

static void msg(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    fflush(stderr);  // Force stderr to flush output
}

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    fflush(stderr);  // Force stderr to flush output
    abort();
}

// Client logic to handle communication with the server
int main() {
    // Create a socket for communication
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }

    // Specify server address details
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);  // Server port number
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP address (localhost)

    // Connect to the server
    int rv = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (rv < 0) {
        die("connect()");
    }

    char choice_buffer[2] = {0};  // Buffer for sending choice (only expecting one character)
    char response_buffer[1024] = {0};  // Buffer for receiving messages from server

    // Client communication loop
    while (1) {
        // Receive message from server
        ssize_t n = read(fd, response_buffer, sizeof(response_buffer) - 1);  // Read server message
        if (n <= 0) {
            msg("read() error or connection closed by server");
            break;  // Exit if there's an error or server closes the connection
        }

        // Null-terminate the message and print it
        response_buffer[n] = '\0';  // Null-terminate the received message
        printf("%s", response_buffer);  // Print the server message
        fflush(stdout);

        // Prompt the user to make a choice
        printf("Enter your choice: ");
        fflush(stdout);
        choice_buffer[0] = getchar();  // Read one character from user input

        // Send the user's choice to the server
        if (write(fd, choice_buffer, 1) < 0) {
            msg("write() error");
            break;
        }

        // Clear newline character from input buffer
        getchar();  // This captures the newline after the user's input
    }

    // Close the socket and exit
    close(fd);
    return 0;
}
