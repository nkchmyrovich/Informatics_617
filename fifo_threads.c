#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
дайте переменным и ф-ям чуть более осмысленные названия:
mythread_1 -> receiveMessages
mythread_2 -> sendMessages
и т.п.
можно вычисление 1 - ind вынести в отдельную ф-и и дать ей "говорящее" название 
*/

#define MAX_CHAR_COUNT 255

int fd_1, fd_2;
char* files[] = {"fifo_file_1.fifo", "fifo_file_2.fifo"};

int ClientReceiveId (int id) {
	return id;
}

int ClientSendId (int id) {
	return 1 - id;
}

void* ReceiveMessages (void* client_id) {
	char* buf_1 = (char*)calloc(MAX_CHAR_COUNT, sizeof(char));
	if (!buf_1) {
		printf("ERROR: buf_1 not allocated\n"); 
		return NULL;
	}
	if ((fd_1 = open(files[*(int*)client_id], O_WRONLY)) < 0) {
		printf("ERROR: open fd_1\n");
		return NULL;
	}
	while (1) {
		fgets(buf_1, MAX_CHAR_COUNT, stdin);
		write(fd_1, (void*)buf_1, MAX_CHAR_COUNT);
	}
	free(buf_1);
	
}

void* SendMessages (void* client_id) {
	char* buf_2 = (char*)calloc(MAX_CHAR_COUNT, sizeof(char));
	if (!buf_2) {
                printf("ERROR: buf_2 not allocated\n");
                return NULL;
        }
	if((fd_2 = open(files[*(int*)client_id], O_RDONLY)) < 0) {
		printf("ERROR: open fd_2\n");
		return NULL;
	}
	while (1) {
		read(fd_2, (void*)buf_2, MAX_CHAR_COUNT);
		printf("CLIENT %d: %s\n", *(int*)client_id, buf_2);
	}
	free(buf_2);
	
}

int main (int argv, char** argc) {
	int ind = atoi(argc[1]);
	pthread_t thid_1, thid_2;
	int client_receive_id = ClientReceiveId(ind);
	int client_send_id = ClientSendId(ind);
	if ((access(files[ind], F_OK) != 0) && (access(files[1 - ind], F_OK) != 0)) {
		if (mknod(files[ind], S_IFIFO | 0666, 0) < 0) return 1;
		if (mknod(files[1 - ind], S_IFIFO | 0666, 0) < 0) return 1;
	}
	if (pthread_create(&thid_1, NULL, ReceiveMessages, &client_receive_id) != 0) {
		printf("ERROR: in pthread_create_1");
		return 1;
	}
	if (pthread_create(&thid_2, NULL, SendMessages, &client_send_id) != 0) {
                printf("ERROR: in pthread_create_2");
                return 1;
        }
	pthread_join(thid_1, NULL);
	pthread_join(thid_2, NULL);
	return 0;
}


