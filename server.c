#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 99999
#define MAX_CONNECTION 10

void communicate(int socket_dialog);
void command(int socket_dialog, char* input);
void substring(char s[], char sub[], int p, int l) {
   int c = 0;
 
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
}
int main (int argc, char** argv) {

	int socket_listen, socket_dialog;
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
	socket_listen = socket(AF_INET, SOCK_STREAM, 0);

	if (bind(socket_listen, (struct sockaddr*) &server, sizeof(server)) < 0) {
		printf("Error while binding");
		exit(-1);
	}

	listen(socket_listen, MAX_CONNECTION);

	ssize_t nb_char;
	int clients_count = 0, client_number = 0;
	for (;;) {

		client_len = sizeof(client);

		socket_dialog = accept(socket_listen, (struct sockaddr*) &client, &client_len);

		client_number++;

		if (fork()) { // Parent
			close(socket_dialog);
			clients_count++;
	
			printf("\n%d client(s) connected\n", clients_count);
		}
		else { // Child
			close(socket_listen);
			
			communicate(socket_dialog);

			printf("Client has been deconnected\n");
			
			close(socket_dialog);
			return 0;
		}

	}

	printf("Server stopped\n");

	close(socket_listen);
	return 0;
}

void communicate(int socket_dialog) {

	char reception[BUFFER_SIZE];
	ssize_t nb_char;
	
	while ((nb_char = read(socket_dialog, reception, BUFFER_SIZE) > 0) && (strcmp(reception, "STOP\n") != 0)) {
		
		command(socket_dialog, reception);
	}
}

void command(int socket_dialog, char* input) {

	char emission[BUFFER_SIZE];

	if (strcmp(input, "rls\n") == 0) {
		printf("-- Listing directory\n");
		
		FILE *fp;
		char path[1035];

		fp = popen("/bin/ls /etc/", "r");
		if (fp == NULL) {
			printf("Failed to run command\n" );
			exit(1);
		}

		while (fgets(path, sizeof(path)-1, fp) != NULL) {
			strcat(emission, path);
		}
			
		write(socket_dialog, emission, strlen(emission) + 1);

		pclose(fp);
	}
	else if (strstr(input, "rcd") != NULL) {	
		char path[1035];
		substring(input, path, 5, strlen(input) - 4);

		printf("-- Changing directory to ");
		printf("%s\n", path);

		if (chdir(path) == 0) {
			command(socket_dialog, "CDOK");
		}
		else {
			command(socket_dialog, "NOCD");
		}
	}
	else if (strcmp(input, "rpwd\n") == 0) {
		printf("-- Current directory\n");
	
		FILE* file;
		char path[1035];

		file = popen("/bin/pwd", "r");

		while (fgets(path, sizeof(path)-1, file) != NULL) {
			strcat(emission, path);
		}
			
		write(socket_dialog, emission, strlen(emission) + 1);

		pclose(file);
	}
	else if (strstr(input, "upld") != NULL) {
		char path[1035];
		substring(input, path, 6, strlen(input) - 5);
		
		printf("-- Upload file: ");
		printf("%s\n", path);
		
		command(socket_dialog, "Uploaded");
	}
	else if (strstr(input, "downl") != NULL) {
		char path[1035];
		char content[BUFFER_SIZE];
		FILE* file;
		substring(input, path, 7, strlen(input) - 6);

		printf("-- Download file: ");
		printf("%s\n", path);
		
		file = fopen(path, "r");
		if (file != NULL) {
			while (fgets(content, BUFFER_SIZE, file) != NULL) {
				strcat(emission, content);
			}
		}
		else {
			strcpy(emission, "UNKWN");
		}
		
		write(socket_dialog, emission, strlen(emission) + 1);

		pclose(file);
	}
	else {
		printf("%s\n", input);
		write(socket_dialog, input, strlen(input) + 1);
	}
}

