#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const char delimiter_line[] = "\n";
const char delimiter_cmd[] = " ";

void Split(char* string, char* delimiters, char** tokens, int* tokensCount);

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
	char** params = (char**)calloc(len, sizeof(char*));
	int line_count = 0;
	int token_count = 0;
	int param_count = 0;
	Split(file_str, delimiter_line, lines, &line_count);
	printf("lins: %d \n", line_count);
	for (size_t i = 0; i < line_count; i++) {
		Split(lines[i], delimiter_cmd, tokens, &token_count);
		printf("tok:%d\n", token_count);
		for (size_t j = 1; j < token_count - 1; j++) {
			params[param_count] = tokens[j];
			printf("%s ", tokens[j]);
			param_count++;
		}
		pid_t pid = fork();
		if (pid == 0) {
			sleep(atoi(tokens[token_count-1]));
			/*
			fixit: зачем вам нужно копирование в отдельный массив params?
			чем tokens + 1 не подойдет?
			*/
			execvp(tokens[0], params);
			token_count = 0;
			param_count = 0;
			memset(tokens, '\0', len);
			memset(params, '\0', len);
		} else {
			/*
			fixit: у вас как-то странно получается: если процесс, которые вы запустили exec'ом "завис", то
			вы не запустите следующую задачу из списка вовремя
			*/
			wait();
		}
	}
	/*
	fixit: допустим ни одна задача не смогла запуститься с помощью exec ...
	посчитайте сколько раз был вызван calloc и сколько раз вы освободили память ...
	эти числа должны совпадать
	*/
	for (size_t i = 0; i < len; i++) {
		free(lines[i]);
		free(tokens[i]);
		free(params[i]);
	}
	free(file_str);
	free(lines);
	free(tokens);
	free(params);
	return 0;
}

void Split(char* string, char* delimiters, char** tokens, int* tokensCount) {
	size_t len = strlen(string);
	int token_count = 0;
	tokens[token_count] = (char*)calloc(len, sizeof(char));
	tokens[token_count++] = strtok(string, delimiters);
	do {
		tokens[token_count] = (char*)calloc(len, sizeof(char));
	} while ((tokens[token_count++] = strtok(NULL, delimiters)) != NULL && token_count < len);
	if (!tokens[token_count-1]) token_count--;
	*tokensCount = token_count;
}
