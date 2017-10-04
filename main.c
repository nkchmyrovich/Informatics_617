#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void GenerateString (int n) {
	char** string_array = (char**)calloc(n, sizeof(char**));
	string_array[0] = (char*)calloc(1, sizeof(char*));
	string_array[0] = "a";
	size_t len = 1;
	char letter = 'b';
	for (size_t i = 1; i < n; i++) {
		string_array[i] = (char*)calloc(2*len+1, sizeof(char*));
		string_array[i] = strncat(string_array[i], string_array[i-1], len);
		string_array[i] = strncat(string_array[i], &letter, 1); 
		string_array[i] = strncat(string_array[i], string_array[i-1], len);
		len = 2*len+1;
		letter = letter + 1;
	}
	for (size_t i = 0; i < n; i++) {
		printf("%s\n", string_array[i]);
	}
}

void Split(char* string, char letter) {
	bool exit = false;
	void** string_array = (void**)calloc(strlen(string), sizeof(void**));
	string_array[0] = string;
	size_t word_count = 0; 
	for (size_t i = 0; i < strlen(string); i++) {
		if ((string[i] == letter)||(exit = (string[i] == '\0'))) {
			if (exit) break;
			word_count++;
			printf("%d ", word_count);
			/*string[i] = '\0';*/
			string_array[word_count] = string + i + 1;
		}`
	}
	for (size_t i = 0; i < word_count; i++) {
		printf("%s\n", string_array[i]);
	}
}

int main(int argc, char **argv)
{
	GenerateString(atoi(argv[1]));
	char* buf = "ABCDEFDESR";
	Split(buf, 'D');
	system("PAUSE");
	return 0;
}
