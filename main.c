#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void GenerateString (int n) {
	char** string_array = (char**)calloc(n, sizeof(char*));
	string_array[0] = (char*)calloc(1, sizeof(char));
	string_array[0] = "a";
	size_t len = 1;
	char letter = 'b';
	for (size_t i = 1; i < n; i++) {
		string_array[i] = (char*)calloc(2*len+1, sizeof(char));
		string_array[i] = strncat(string_array[i], string_array[i-1], len);
		string_array[i] = strncat(string_array[i], &letter, 1); 
		string_array[i] = strncat(string_array[i], string_array[i-1], len);
		len = 2*len+1;
		letter = letter + 1;
	}
	for (size_t i = 0; i < n; i++) {
		printf("%s\n", string_array[i]);
	}
	//fixed
	for (size_t i = 0; i < n; i++) {
                if (string_array[i] != NULL) free(string_array[i]);
	}
	free(string_array);
}

int main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	GenerateString(n);
	system("PAUSE");
	/*
	fixit: нужно почистить всю память, которые выделяли calloc'ами.
	p.s. почитайте, что realloc делает
	*/
	return 0;
}
