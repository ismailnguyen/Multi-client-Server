#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define MAX_CONNECTION 10

void communicate(int socket_dialog);

int main (int argc, char** argv) {

	int sock_ecoute, sock_dialogue;
	struct sockaddr_in client, server;
	socklen_t client_len;
	int res, port;
	char reception[BUFFER_SIZE], emission[BUFFER_SIZE];

	if ((argv[1] != NULL))
	{
		port = atoi(argv[1]);
	}
	else
	{
		printf("Port:\n");
		scanf("%d", &port);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	printf("Server ready \n");
	sock_ecoute = socket(AF_INET, SOCK_STREAM, 0);

	if (bind(sock_ecoute, (struct sockaddr*) &server, sizeof(server)) < 0) {
		printf("Error while binding");
		exit(-1);
	}

	listen(sock_ecoute, MAX_CONNECTION);

	ssize_t nb_char;
	int clients_count = 0, client_number = 0;
	for (;;) {

		client_len = sizeof(client);

		sock_dialogue = accept(sock_ecoute, (struct sockaddr*) &client, &client_len);

		client_number++;

		if (fork()) { // Parent
			close(sock_dialogue);
			clients_count++;
	
			printf("\n%d client(s) connected\n", clients_count);
		}
		else { // Child
			close (sock_ecoute);
			
			communicate(sock_dialogue);

			printf("Client has been deconnected\n");
			
			close(sock_dialogue);
			return 0;
		}

	}

	printf("Server stopped\n");

	close(sock_ecoute);
	return 0;
}

void communicate(int socket_dialog) {

	char reception[BUFFER_SIZE], emission[BUFFER_SIZE];
	ssize_t nb_char;
	
	while ((nb_char = read(socket_dialog, reception, BUFFER_SIZE) > 0) && (strcmp(reception, "STOP\n") != 0)) {		
		printf("%s\n", reception);
		
		strcpy(emission, "Server acknowledge\n");
		write(socket_dialog, emission, strlen(emission) + 1);
	}
}
