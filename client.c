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

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);  // Server port number
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP address (localhost)
    int rv = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (rv < 0) {
        die("connect()");
    }
    char response_buffer[1000] = {0};
    char input_buffer[256] = {0};
    while (1) {
        ssize_t n = read(fd, response_buffer, sizeof(response_buffer) - 1);  // Read server message
        if (n <= 0) {
            msg("read() error or connection closed by server");
            break;  // Exit if there's an error or server closes the connection
        }

        response_buffer[n] = '\0';  // Null-terminate the received message
        printf("%s", response_buffer);  // Print the server message
        fflush(stdout);
        if(strstr(response_buffer, "Enter") || strstr(response_buffer, "choice")) {
          printf("enter your input: ");
          fflush(stdout);
          if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
              input_buffer[strcspn(input_buffer, "\n")] = '\0';
              if (write(fd, input_buffer, strlen(input_buffer)) < 0) {
                msg("write() error");
                break;
              }
          }
        }
    }
    // Close the socket and exit
    close(fd);
    return 0;
}
