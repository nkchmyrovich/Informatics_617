#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include "morse.h"

#define MAX_NAME_LEN 20
#define MAX_CLIENT_AMOUNT 10
#define STR_LEN 1000

#define EXIT_SOCKET perror(NULL);\
close(sockfd);\
exit(1);

typedef struct {
	char client_name[MAX_NAME_LEN];
	int port;
} Client;

typedef struct {
	int port;
	char* name;
	MorseChar* morse_table;
        char* morse_text;
	int socket;
} ClientArgs;
	
void* ClientServe (void* client_args) {
	int clilen, n;
        char line[STR_LEN];

	int bind_port = ((ClientArgs*)client_args)->port;
	int sockfd = ((ClientArgs*)client_args)->socket;
	MorseChar* morse_table = ((ClientArgs*)client_args)->morse_table;
	char* morse_text = ((ClientArgs*)client_args)->morse_text;
	char* client_name = ((ClientArgs*)client_args)->name;

        struct sockaddr_in servaddr, cliaddr;
	
	printf("Thread info:\n--client name: %s\n--port: %d\n", client_name, bind_port);
	
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(bind_port);
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
                EXIT_SOCKET
        }
        while(1) {
                clilen = sizeof(cliaddr);
                if ((n = recvfrom(sockfd, line, STR_LEN, 0, (struct sockaddr*) &cliaddr, &clilen)) < 0) {
                        EXIT_SOCKET
                }
		printf("Received string: %s", line);
                TranslatorToMorse((char*)line, morse_table, morse_text);
                printf("Morse string: %s\n", morse_text);
                if (sendto(sockfd, morse_text, STR_LEN, 0, (struct sockaddr*) &cliaddr, clilen) < 0) {
                        EXIT_SOCKET
                }
		memset(morse_text, '\0', strlen(morse_text));
        }
}

int main () {
	FILE* morse_table_file = fopen("morse_table.txt", "r");
        if (!morse_table_file) {
                printf("Morse file can not be opened.\n");
                return 0;
        }
        int file_size = FileSize(morse_table_file);
        MorseChar* morse_table = (MorseChar*)calloc(file_size, sizeof(MorseChar));
        GetMorseTable(morse_table_file, morse_table);
	

	FILE* client_table = fopen("client_table.txt", "r");
	if (!client_table) {
		printf("Client table can not be opened.\n");
		return 0;
	}
	Client* clients = (Client*)calloc(MAX_CLIENT_AMOUNT, sizeof(Client));


	size_t i = 0;
	while ((fscanf(client_table, "%[^|]|%d\n", clients[i].client_name, &clients[i].port) != EOF) && (i < MAX_CLIENT_AMOUNT)) {
                i++;
	}
	int client_num = i;
	char** morse_texts = (char**)calloc(client_num, sizeof(char*));
	for (size_t j = 0; j < client_num; j++) {
		morse_texts[j] = (char*)calloc(MAX_MORSE_MESSAGE, sizeof(char));
	}
	ClientArgs* client_args = (ClientArgs*)calloc(client_num, sizeof(ClientArgs));
	pthread_t* clients_thids = (pthread_t*)calloc(client_num, sizeof(pthread_t));
	for (i = 0; i < client_num; i++) {
		client_args[i].port = clients[i].port;
		client_args[i].name = clients[i].client_name;
		client_args[i].morse_table = morse_table;
		client_args[i].morse_text = morse_texts[i];
		if ((client_args[i].socket = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
         	        perror(NULL);
                	exit(1);
        	}
		if (pthread_create(&clients_thids[i], NULL, ClientServe, &client_args[i]) != 0) {
			printf("Error in pthread_create\n");
			return 1;
		
}
	}
	for (i = 0; i < client_num; i++) {
		pthread_join(clients_thids[i], NULL);
	}
	for (size_t j = 0; j < client_num; j++) {
		free(morse_texts[j]);
	}
	free(morse_texts);
	free(morse_table);
	free(clients);	
	return 0;
}
