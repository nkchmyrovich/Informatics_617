#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <ctype.h>
 
#define MAX_CHAR_NUMBER 255
#define TABLE_LIMIT 2

struct sembuf mybuf;

typedef struct {
	char* type;
	char* time;
} Dishes;

int GetFileSize (FILE* file) {
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}

void  SetSemaphore(int semid, int N) { 
	mybuf.sem_op = N;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 0;
        if (semop(semid, &mybuf, 1) < 0) {
                printf("Can`t wait for condition\n");\
                exit(-1);
        }
}

int main () {
	int status;
	int fd[2];
	FILE* file_dishes = fopen("wash_dishes.txt", "r");
	FILE* file_washer = fopen("washer.txt", "r");
	FILE* file_wiper = fopen("wiper.txt", "r");
	
	int fsize_dishes = GetFileSize(file_dishes);
	int fsize_washer = GetFileSize(file_washer);
	int fsize_wiper = GetFileSize(file_wiper);
	printf("File sizes: %d %d %d\n", fsize_dishes, fsize_washer, fsize_wiper);

	char* washer_buf_type = (char*)calloc(fsize_washer, sizeof(char));
	char* wiper_buf_type = (char*)calloc(fsize_wiper, sizeof(char));
	char* washer_buf_count = (char*)calloc(fsize_washer, sizeof(char));
	
	Dishes* washer_dishes = (Dishes*)calloc(fsize_dishes, sizeof(Dishes));
	Dishes* wiper_dishes = (Dishes*)calloc(fsize_dishes, sizeof(Dishes));
	
	for (size_t j = 0; j < fsize_dishes; j++) {
		washer_dishes[j].type = (char*)calloc(MAX_CHAR_NUMBER, sizeof(char));
		washer_dishes[j].time = (char*)calloc(MAX_CHAR_NUMBER, sizeof(char));
		wiper_dishes[j].type = (char*)calloc(MAX_CHAR_NUMBER, sizeof(char));
		wiper_dishes[j].time = (char*)calloc(MAX_CHAR_NUMBER, sizeof(char));
	}
	size_t i = 0;
	while(fscanf(file_washer, "<%[^>]>:<%[^>]>\n", washer_dishes[i].type, washer_dishes[i].time) != EOF)
		i++;
	int washer_count = i;
	i = 0;
	while(fscanf(file_wiper, "<%[^>]>:<%[^>]>\n", wiper_dishes[i].type, wiper_dishes[i].time) != EOF)
                i++;
	int wiper_count = i;
	i = 0;
	
	int semid;
        char* pathname = "key_file.c";
        key_t key;
        struct sembuf mybuf;
        if ((key = ftok(pathname, 0)) < 0) {
                printf("Can`t get key\n");
                return 1;
        }
        if ((semid = semget(key, 1, 0666|IPC_CREAT)) < 0) {
                printf("Can`t create shm\n");
                return 1;
        }

        SetSemaphore(semid, TABLE_LIMIT);

	pipe(fd);

	pid_t pid = fork();
	if (pid > 0) {
		while (fscanf(file_dishes, "<%[^>]>:<%[^>]>\n", washer_buf_type, washer_buf_count) != EOF) {
			size_t count = atoi(washer_buf_count);
			for (size_t j = 0; j < washer_count; j++) {
				if (!strcmp(washer_dishes[j].type, washer_buf_type)) {
					for (size_t k = 0; k < count; k++) {
						sleep(atoi(washer_dishes[j].time));
						SetSemaphore(semid, -1);
						printf("Washer: <%s>:<%s>\n", washer_dishes[j].type, washer_dishes[j].time);
						write(fd[1], washer_buf_type, MAX_CHAR_NUMBER);
					}
					break;
				}
			//memset(washer_buf_type, '\0', fsize_washer);
			//memset(washer_buf_count, '\0', fsize_washer);
                	i++;
			}
		}
		write(fd[1], "finish\0", 7);
		wait(NULL);
	} 
	if (pid == 0) {
		read(fd[0], wiper_buf_type, MAX_CHAR_NUMBER);
		while (strcmp(wiper_buf_type, "finish\0") != 0) {
		for (size_t j = 0; j < wiper_count; j++) {
			if (!strcmp(wiper_dishes[j].type, wiper_buf_type)) {
                                sleep(atoi(wiper_dishes[j].time));
				SetSemaphore(semid, 1);
                                printf("Wiper: <%s>:<%s>\n", wiper_dishes[j].type, wiper_dishes[j].time);
                                break;
                         }
		}
		//memset(wiper_buf_type, '\0', fsize_wiper);
		read(fd[0], wiper_buf_type, MAX_CHAR_NUMBER);
		}
	} 
	semctl(semid, IPC_RMID, 0);	
	return 0;
}
