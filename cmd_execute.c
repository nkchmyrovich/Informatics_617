#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

char delimiter_line[] = "\n";
char delimiter_cmd[] = " ";

void Split(char* string, char* delimiters, char** tokens, int* tokensCount) {
        size_t len = strlen(string);
        int token_count = 0;
        tokens[token_count++] = strtok(string, delimiters);
        while ((tokens[token_count++] = strtok(NULL, delimiters)) != NULL && token_count < len) {};
        if (!tokens[token_count-1]) token_count--;
        *tokensCount = token_count;
}

int main(int argc, char** argv) {
	FILE *file_cmd = fopen("file_cmd.txt", "r");
	if (!file_cmd) return -1;
	fseek(file_cmd, 0, SEEK_END);
	size_t file_size = ftell(file_cmd);
	fseek(file_cmd, 0, SEEK_SET);
	char* file_str = (char*)calloc(file_size, sizeof(char));
	fread(file_str, sizeof(char), file_size, file_cmd);
	size_t str_counter = 0;
	for (size_t i = 0; i < file_size; i++) {
		if (file_str[i] == '\n') str_counter++;
	}
	size_t len = strlen(file_str);
	char** lines = (char**)calloc(len, sizeof(char*));
	char** tokens = (char**)calloc(len, sizeof(char*));
	int line_count = 0;
	int token_count = 0;
	int param_count = 0;
	Split(file_str, delimiter_line, lines, &line_count);
	for (size_t i = 0; i < line_count; i++) {
		Split(lines[i], delimiter_cmd, tokens, &token_count);
		pid_t pid = fork();
		if (pid == 0) {
			sleep(atoi(tokens[token_count-1])); 
			tokens[token_count - 1] = NULL;
			execvp(tokens[0], tokens);
			token_count = 0;
			param_count = 0;
		}
	}
	free(file_str);
	free(lines);
	free(tokens);
	wait(NULL);
	return 0;
}
