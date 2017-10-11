#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100

void Split(char* string, char* delimiters, char** tokens, int* tokensCount);

int main(int argc, char **argv) {
	char* string = (char*)calloc(N, sizeof(char));
	char* delimiter = (char*)calloc(N, sizeof(char));
	scanf("%[^\n]s ", string);
	fflush(stdin);
	scanf("%[^\n]s ", delimiter);
	if (!(delimiter && string)) {
		return -1;
	}
	char** tokens = (char**)calloc(strlen(string), sizeof(char*));
	int token_count = 0;
	Split(string, delimiter, tokens, &token_count);
	for (size_t i = 0; i < token_count; i++) {
		printf("%s ", tokens[i]);
	}
	system("PAUSE");
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