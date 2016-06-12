#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUFFER_SIZE 1024

void communicate(FILE* file, int socket_dialog);

int main (int argc, char *argv[]) {

	struct sockaddr_in server;
	char ip_address[16], file_type[200];
	int socket_client, res, port;
	FILE* file;
	char ;

	socket_client = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_client == -1) {
		printf("Error during TCP socket establishment\n");
		exit (10);
	}

	if ((argv[1] != NULL) && (argv[2] != NULL) && (argv[3] != NULL)) {
		strcpy(ip_address, argv[1]);
		port = atoi(argv[2]);
		strcpy(file_type, argv[3]);
	}
	else {
		printf("IP address:\n");
		scanf("%s", ip_address);
			
		printf("Port:\n");
		scanf("%d", &port);
		
		printf("Source:\n");
		scanf("%s", file_type);
	}
	
	
	if (strcmp(file_type, "stdin") != 0) {
		file = fopen(file_type, "r");
	}
	else {
		file = stdin;
	}
	
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip_address);
	server.sin_port = htons(port);
	
	res = connect(socket_client, (struct sockaddr*) &server, sizeof(server));
	if (res == -1){
		printf("Error while connecting\n");
		exit (70);
	}
	
	printf("Connection established\n");
	
	communicate(file, socket_client);
	
	printf("Closing socket : ");
	res = close(socket_client);
	if (res == -1) {
		printf("Error while closing socket #%d\n", socket_client);
		exit (90);
	}

	printf("Socket #%d closed\n", socket_client);

	printf("Client stopped\n");
	return 0;
}

void communicate(FILE* file, int socket_dialog) {
	
	char reception[BUFFER_SIZE], emission[BUFFER_SIZE];
	int nb_char;
	
	while ((fgets(emission, BUFFER_SIZE, file) != NULL) && (strcmp(emission, "STOP\n") != 0)) {
		
		write(socket_dialog, emission, strlen(emission) + 1);
		
		nb_char = read(socket_dialog, reception, BUFFER_SIZE);
		
		fputs(reception, stdout);
	}
}
