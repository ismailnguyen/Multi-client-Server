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

int main (int argc, char *argv[]) {

	struct sockaddr_in server;
	char ip_address[16];
	int socket_client, res, port, i;
	char buffer[BUFFER_SIZE];
	socklen_t socket_len;

	socket_client = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_client == -1){
		printf("Error during TCP socket establishment\n");
		exit (10);
	}

	if((argv[1] != NULL) && (argv[2] != NULL))
	{
		strcpy(ip_address, argv[1]);
		port = atoi(argv[2]);
	}
	else
	{
		printf("IP address:\n");
		scanf("%s", ip_address);
			
		printf("Port:\n");
		scanf("%d", &port);
	}
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip_address);
	server.sin_port = htons(port);
	socket_len = sizeof(server);

	
	res = connect(socket_client, (struct sockaddr*) &server, socket_len);
	if (res == -1){
		printf("Error while connecting\n");
		exit (70);
	}
	printf("Connection established\n");
	
	do
	{
	  for(i=0; i<BUFFER_SIZE; i++)
	      buffer[i] = 0x00;
	
	  fgets(buffer, BUFFER_SIZE, stdin);
	  
	
	  send(socket_client, buffer, strlen(buffer), 0);
	}
	while((strcmp(buffer, "STOP\n") != 0) && (strcmp(buffer, "stop\n") != 0));
	
	
	
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
