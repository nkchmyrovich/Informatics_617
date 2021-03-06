#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100

void Split(char* string, char* delimiters, char** tokens, int* tokensCount);

int main(int argc, char **argv) {
	char* string = (char*)calloc(N, sizeof(char));
	char* delimiter = (char*)calloc(N, sizeof(char));
	fgets(string, N, stdin);
	fflush(stdin);
	fgets(delimiter, N, stdin);
	if (!(delimiter && string)) {
		return -1;
	}
	char** tokens = (char**)calloc(strlen(string), sizeof(char*));
	int token_count = 0;
	Split(string, delimiter, tokens, &token_count);
	for (size_t i = 0; i < token_count; i++) {
		printf("%s ", tokens[i]);
	}
	free(string);
	free(delimiter);
	free(tokens);
	return 0;
}

void Split(char* string, char* delimiters, char** tokens, int* tokensCount) {
	size_t len = strlen(string);
	int token_count = 0;
	tokens[token_count++] = strtok(string, delimiters);
	while ((tokens[token_count++] = strtok(NULL, delimiters)) != NULL && token_count < len) {};
	if (!tokens[token_count-1]) token_count--;
	*tokensCount = token_count;
}
