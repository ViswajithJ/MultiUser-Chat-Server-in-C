#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 60000
#define BUFFER_SIZE 256

char buffer[BUFFER_SIZE];
char name[BUFFER_SIZE];
int client_sockfd;
struct sockaddr_in serv_addr;

void *send_msg() {
	while(1) {
		memset(buffer, 0, BUFFER_SIZE);
		fgets(buffer, BUFFER_SIZE, stdin);
		
		send(client_sockfd, buffer, strlen(buffer), 0);
		if (!strcmp(buffer, "quit\n")) {
			exit(1);
		}
	}
}

void *recv_msg() {
	while(1) {
		memset(buffer, 0, BUFFER_SIZE);
		if(recv(client_sockfd, buffer, BUFFER_SIZE, 0) <= 0) {
			printf("Connection disconnected\n");
			exit(1);
		}
		printf("%s", buffer);
	}
}

int main()
{
	// DECLARING STUFF
	struct sockaddr_in serv_addr;

	// SOCKET STUFF
	client_sockfd = socket(AF_INET, SOCK_STREAM,  0);
    if (client_sockfd == -1) {
        printf("Socket error\n");
        exit(1);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (connect(client_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <  0) {
        printf("Connect error\n");
        exit(1);
    }

	printf("Start chatting. Type quit to close connection\n");

	pthread_t send_msg_t, recv_msg_t;

	pthread_create(&send_msg_t, NULL, send_msg, NULL);
	pthread_create(&recv_msg_t, NULL, recv_msg, NULL);

	pthread_join(send_msg_t, NULL);
    pthread_join(recv_msg_t, NULL);

    close(client_sockfd);

}