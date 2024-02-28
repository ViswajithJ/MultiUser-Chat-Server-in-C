#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>


char buffer[256];
char name[256];
int client_sockfd;
struct sockaddr_in serv_addr;

void *send_msg() {
	char buffer[256];
	while(1) {
		memset(buffer, 0, sizeof(buffer));
		fgets(buffer, 256, stdin);
		
		send(client_sockfd, buffer, strlen(buffer), 0);
		if (!strcmp(buffer, "quit\n")) {
			break;
		}
		// sleep(2);
	}
}

void *recv_msg() {
	while(1) {
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
			int n = recv(client_sockfd, buffer, 256, 0);	
			printf("%s", buffer);
			if (n == 0)
				break;
	}
}

int main()
{
	// DECLARING STUFF
	char buffer[256];
	char name[256];

	struct sockaddr_in serv_addr;

	// SOCKET STUFF
	client_sockfd = socket(AF_INET, SOCK_STREAM,  0);
    if (client_sockfd == -1) {
        printf("Socket error\n");
    }

    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(60000);

    if (connect(client_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <  0) {
        printf("Connect error\n");
        return 1;
    }

    // server asking for name
    memset(buffer, 0, sizeof(buffer));
	recv(client_sockfd, buffer, 256, 0);
	printf("%s", buffer);

	// sending name to server
	memset(name, 0, sizeof(name));
	fgets(name, 256, stdin);
	send(client_sockfd, name, strlen(name), 0);

	name[strlen(name)-1] = '\0';
	printf("Start chatting. Type quit to close connection\n");

	pthread_t send_msg_t;
	pthread_create(&send_msg_t, NULL, (void *)send_msg, NULL);

	pthread_t recv_msg_t;
	pthread_create(&recv_msg_t, NULL, (void *)recv_msg, NULL);

	pthread_join(send_msg_t, NULL);
    pthread_join(recv_msg_t, NULL);

    close(client_sockfd);

}