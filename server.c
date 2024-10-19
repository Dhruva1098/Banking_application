#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#include "headers/client_handler.h"

static void msg(const char *msg){
	fprintf(stderr, "%s\n", msg);
	fflush(stderr);
}

static void die(const char *msg){
	int err = errno;
	fprintf(stderr, "[%d] %s\n", err, msg);
	fflush(stderr);
	abort();
}
int main(){
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd<0){
		die("socket()");
	}	
	int val = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)); 
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = ntohs(1234);
	addr.sin_addr.s_addr = ntohl(0); //0.0.0.0
	int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
	if (rv){
		die("bind()");
	}
	rv = listen(fd, SOMAXCONN);
	if(rv){
		die("listen()");
	}
	while(1){
		struct sockaddr_in client_addr = {};
		socklen_t socklen = sizeof(client_addr);
		int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
		client_handler(connfd);
    close(connfd);
	}
	close(fd);
	return 0;

}
