#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include "morse.h"

#define STR_LEN 1000
#define EXIT_SOCKET perror(NULL);\
close(sockfd);\
exit(1);

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Wrong amount of cmd arguments.\n");
		return 1;
	}
	int sockfd;
	int n, len;
	char recvline[STR_LEN], sendline[STR_LEN];
	struct sockaddr_in servaddr, cliaddr;
	int bind_port = atoi(argv[2]);

	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror(NULL);
		exit(1);
	}

	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;	
	cliaddr.sin_port = htons(0);
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(sockfd, (struct sockaddr*) &cliaddr, sizeof(cliaddr)) < 0) {
		EXIT_SOCKET
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(bind_port);
	
	if (inet_aton(argv[1], &servaddr.sin_addr) == 0) {
		EXIT_SOCKET
	}
	while (1) {
		printf("Enter the text to translate to morse code => \n");
		fflush(stdin);
		fgets(sendline, STR_LEN, stdin);
		if (sendto(sockfd, sendline, STR_LEN+1, 0, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
			EXIT_SOCKET
		}
		if ((n = recvfrom(sockfd, recvline, STR_LEN, 0, (struct sockaddr*) NULL, NULL)) < 0) {
			EXIT_SOCKET
		}
		SignalExecutor(recvline);
	}
	close (sockfd);
	return 0;
}
