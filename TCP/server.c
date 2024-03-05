#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 60000
#define BUFFER_SIZE 256
#define MAX_CLIENTS 5

int client_sockfds[5];  // array of client socket fds
int client_count = 0;

void *client_chat(void *client_sock)
{
	int c_sock = *(int *)client_sock;
	char buffer[BUFFER_SIZE];

 	while(1)
 	{
 		memset(buffer, 0, sizeof(buffer));
 		int read_bytes = recv(c_sock, buffer, BUFFER_SIZE, 0);
 		if ((!strcmp(buffer, "quit\n")) || (read_bytes == 0)) {
 			close(c_sock);
 			printf("Connection disconnected\n");
 			client_count--;
 			if (!client_count)
 				exit(1);
 		} else if (read_bytes < 0) {
 			printf("recv error \n");
 			break;
 		} else {
 			for (int i = 0; i < client_count; i++) {
 				if (client_sockfds[i] != c_sock) {
 					int e = send(client_sockfds[i], buffer, strlen(buffer), 0);
 					if (e < 0)
 						printf("Send error \n");
 				}
 			}
 		}
 	}
 	free(client_sock);
    pthread_exit(NULL);
}

int main()
{
	// DECLARE STUFF
	int serv_sockfd;
	struct sockaddr_in serv_addr, client_addr;

	//SOCKET
	serv_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sockfd < 0) {
		printf("Socket error\n");
		exit(1);
	}
	
	//BIND
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);

			 //(server struct, address of struct, size of struct)
	if (bind(serv_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Binding error \n");
		exit(1);
	}
	//LISTEN
	       //server fd, max requests(clients))
	listen(serv_sockfd, MAX_CLIENTS);
	printf("server is listening on port %d\n", PORT);

	//ACCEPT
	int client_len = sizeof(struct sockaddr_in);

	while(1) {
		if ((client_sockfds[client_count] = accept(serv_sockfd, (struct sockaddr *)&client_addr, &client_len)) < 0)
		{
			printf("Accept error\n");
			exit(1);
		}
		client_count++;
		if (client_count < MAX_CLIENTS) {
			printf("Client %d connected \n", client_count-1);
			pthread_t client_thread;
			int *client_sock = (int *)malloc(sizeof(int));
			*client_sock = client_sockfds[client_count - 1];
							 //address of thread, 
			if(pthread_create(&client_thread, NULL, client_chat, (void *) client_sock) < 0) 
					printf("Thread error \n");
		} else {
			printf("Max Clients Reached. Connection Rejected. Bye\n");
			close(client_sockfds[client_count]);
		}
	}
}