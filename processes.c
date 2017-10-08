#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>

int main (int argc, char** argv) {
	size_t proc_num = atoi(argv[1]);
	int *status;
	pid_t pid;
	for(size_t i = 0; i < proc_num; i++) {
		pid = fork();
		if(pid == 0) {
			printf("Child id: %d\n", getpid());
			printf("Parent id: %d\n", getppid());
		} else if (pid > 0) {
			wait(status);
			printf("Status: %d\n", *status);
			return 1;
		} else {
			printf("ERROR in fork");
		}
	}
	return 0;
}