#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>

#define MAX_STR_LEN 255

int FindFile (char* dir_name, size_t depth, char* file_name, size_t _depth) {
	static bool ignore = false;
	_depth++;
	if (_depth > depth) { 
		return 1; 
	}
	DIR* dir_stream = opendir(dir_name);
        if (!dir_stream) {
                printf("No such directory to open\n");
                return 1;
        }
        struct dirent* dir_rec;
	while (dir_rec = readdir(dir_stream)) {
		if (!strcmp(dir_rec->d_name, ".")||!strcmp(dir_rec->d_name, "..")) {
			ignore = true;
		} else {
			ignore = false;
		}
		if (!ignore) {
			if (!strcmp(dir_rec->d_name, file_name)) {
				printf("File was found.\nDirectory: %s\n", dir_name);
				exit(0);
			}
			if (dir_rec->d_type == DT_DIR) {
				char* new_dir_name = (char*)calloc(MAX_STR_LEN, sizeof(char));
				strcat(new_dir_name, dir_name);
				strcat(new_dir_name, "/");
				strcat(new_dir_name, dir_rec->d_name);
				FindFile(new_dir_name, depth, file_name, _depth);
				free(new_dir_name);
			}
		}
	}
}
	
	

int main(int argc, char** argv) {
	char* file_name = (char*)calloc(MAX_STR_LEN, sizeof(char));
	char* dir_name = (char*)calloc(MAX_STR_LEN, sizeof(char));
	size_t depth = atoi(argv[2]);
	strcpy(file_name, argv[3]);
	strcpy(dir_name, argv[1]);
	if (FindFile(dir_name, depth, file_name, 0)) {
		printf("File not found\n");
	}
	return 0;
}	
	

