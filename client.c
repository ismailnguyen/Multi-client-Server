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
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

#define BUFFER_SIZE 99999

void communicate(int, FILE*);
void command(int, char*);

int main (int argc, char *argv[]) {

	struct sockaddr_in server;
	int socket_client, res;

	socket_client = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_client == -1) {
		printf("Error during TCP socket establishment\n");
		exit (10);
	}
	
	if (argc != 3) {
		printf("Usage: client <IPaddress> <Port>\n");
		exit (-1);
	}

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &server.sin_addr);
	
	res = connect(socket_client, (struct sockaddr*) &server, sizeof(server));
	if (res == -1){
		printf("Error while connecting\n");
		exit (-2);
	}
	
	printf("Connection established\n");
	printf("\> ");
	
	communicate(socket_client, stdin);
	
	printf("Closing socket : ");
	res = close(socket_client);
	if (res == -1) {
		printf("Error while closing socket #%d\n", socket_client);
		exit (-3);
	}

	printf("Socket #%d closed\n", socket_client);

	printf("Client stopped\n");
	return 0;
}

void communicate(int socket_dialog, FILE* file) {
	
	char reception[BUFFER_SIZE], emission[BUFFER_SIZE];
	int nb_char;
	
	while ((fgets(emission, BUFFER_SIZE, file) != NULL) && (strcmp(emission, "STOP\n") != 0)) {
		
		command(socket_dialog, emission);
		
		printf("\n> ");
		
		if ((nb_char = read(socket_dialog, reception, BUFFER_SIZE)) > 0) {		
			fputs(reception, stdout);
		}

	}
}

void command(int socket_dialog, char* input) {

	char emission[BUFFER_SIZE];
	
	if (strcmp(input, "ls\n") == 0) {
		execlp("/bin/ls", "ls", NULL);
	}
	else if (strcmp(input, "pwd\n") == 0) {
		execlp("/bin/pwd", "pwd", NULL);
	}
	else if (strcmp(input, "cd\n") == 0) {
		char path[1035];
		
		printf("Path:\n");
		scanf("%s", &path);
		
		chdir(path);
		
		execlp("/bin/ls", "ls", NULL);
	}
	else if (strcmp(input, "rcd\n") == 0) {
		char path[1035];
		
		printf("Path:\n");
		scanf("%s", &path);
		
		strcpy(emission, "rcd ");
		strcat(emission, path);
		
		write(socket_dialog, emission, strlen(emission) + 1);
	}
	else if (strcmp(input, "rm\n") == 0) {
		char path[1035];
		
		printf("Path:\n");
		scanf("%s", &path);
		
		unlink(path);
		
		execlp("/bin/ls", "ls", NULL);
	}
	else if (strcmp(input, "upld\n") == 0) {
		char path[1035];
		char content[BUFFER_SIZE];
		FILE* file;
		
		printf("File:\n");
		scanf("%s", &path);
		
		strcpy(emission, "upld ");
		strcat(emission, path);
		strcat(emission, "\n");
		
		file = fopen(path, "r");
		while (fgets(content, BUFFER_SIZE, file) != NULL) {
			strcat(emission, content);
		}
		
		write(socket_dialog, emission, strlen(emission) + 1);
	}
	else if (strcmp(input, "downl\n") == 0) {
		char path[1035];
		
		printf("File:\n");
		scanf("%s", &path);
		
		strcpy(emission, "downl ");
		strcat(emission, path);
		
		write(socket_dialog, emission, strlen(emission) + 1);
	}
	else {
		write(socket_dialog, input, strlen(input) + 1);
	}
}
