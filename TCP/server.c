#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>


int client_sockfds[5];  // array of client socket fds
int client_count = 0;

void *client_chat(void *client_sock)
{
	int c_sock = *(int *)client_sock;
	char name[256];
	char buffer[256];

	memset(buffer, 0, sizeof(buffer));
 	strcpy(buffer, "Enter name: \0");
 	send(c_sock, buffer, strlen(buffer), 0);

 	memset(name, 0, sizeof(name));
 	recv(c_sock, name, 256, 0);

 	// name[strlen(name)-1] = ' : ';

 	while(1)
 	{
 		memset(buffer, 0, sizeof(buffer));
 		int read_bytes = recv(c_sock, buffer, 256, 0);
 		if ((!strcmp(buffer, "quit\n")) || (read_bytes == 0)) {
 			close(c_sock);
 			printf("Connection disconnected\n");
 			break;
 		} else if (read_bytes < 0) {
 			printf("recv error \n");
 			break;
 		} else {
 			strcat(name, buffer);
 			for (int i = 0; i < client_count; i++) {
 				if (client_sockfds[i] != c_sock) {
 					int e = send(client_sockfds[i], buffer, strlen(buffer), 0);
 					if (e < 0)
 						printf("Send error \n");
 				}
 			}
 		}
 	}




}

int main()
{
	// DECLARE STUFF
	int serv_sockfd;
	struct sockaddr_in serv_addr, client_addr;

	//SOCKET
	serv_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sockfd < 0)
		printf("Socket error\n");

	//BIND
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(60000);

			 //(server struct, address of struct, size of struct)
	if (bind(serv_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		printf("Binding error \n");


	//LISTEN
	       //server fd, max requests(clients))
	listen(serv_sockfd, 5);
	printf("server is listening on port 60000\n");

	//ACCEPT
	int client_len = sizeof(struct sockaddr_in);

	int i = 0;
	while(client_sockfds[i++] = accept(serv_sockfd, (struct sockaddr *)&client_addr, &client_len)) {
		printf("Client %d connected \n", i-1);
		pthread_t client_thread;
		client_count++;
		int *client_sock = (int *)malloc(sizeof(int));
		*client_sock = client_sockfds[i - 1];

						 //address of thread, 
		if(pthread_create(&client_thread, NULL, client_chat, (void *) client_sock) < 0) 
			printf("Thread error \n");
	}
}