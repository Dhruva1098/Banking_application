#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

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

void customer_handler(int connfd) {
	char* response = "Handling Customer\n";
	write(connfd, response, strlen(response));
}
void bank_employee_handler(int connfd){
	char* response = "Handling bank employee\n";
	write(connfd, response, strlen(response));
}

void manager_handler(int connfd) {
	char* response = "Handling Manager\n";
	write(connfd, response, strlen(response));
}

void admin_handler(int connfd) {
	char* response = "Handling Admin\n";
	write(connfd, response, strlen(response));
}


void client_handler(int connfd) {	
	char wbuffer[] = "Login as:\n1) Customer\n2) Bank Employee \n3) Manager\n4) Administrator\n5) exit\n";
	char rbuffer[2] = {0};
	char invalid_choice[] = "Invalid choice, please try again";
		write(connfd, wbuffer, strlen(wbuffer));
		ssize_t n = read(connfd, rbuffer, sizeof(rbuffer) - 1);
		if (n < 0){
			msg("read() error");
			return;
		}
		fflush(stdout);
		int choice = rbuffer[0] - '0';
		if (choice >= 1 && choice <= 5) {
			switch(choice) {
				case 1:
					customer_handler(connfd);
					break;
				case 2:
					bank_employee_handler(connfd);
					break;
				case 3:
					manager_handler(connfd);
					break;
				case 4:
					admin_handler(connfd);
					break;
				case 5: 
					printf("client chose to exit\n");
					fflush(stdout);
					close(connfd);
					return;
				default:
					break;
			}
		} else {
			write(connfd, invalid_choice, strlen(invalid_choice));
		}
	close(connfd);
}

int main(){
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd<0){
		die("socket()");
	}	
	// AF_INET is for ipv4, you can use AF_INET6 for ipv6 socket
	// SOCK_STREAM is for TCP
	int val = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)); 
	//setsockopt() configures a socket. SO_REUSEADDR server binds to same address if restarted
	//now lets bind and listen, using address 0.0.0.0:1234
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = ntohs(1234);
	addr.sin_addr.s_addr = ntohl(0); //0.0.0.0
	int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
	if (rv){
		die("bind()");
	}
	//listen 
	rv = listen(fd, SOMAXCONN);
	if(rv){
		die("listen()");
	}
	//do something (simple read and write while(true)
	while(1){
		struct sockaddr_in client_addr = {};
		socklen_t socklen = sizeof(client_addr);
		int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
		client_handler(connfd);
	}
	close(fd);
	return 0;

}
