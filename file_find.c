#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_STR_LEN 255
#define PARAMAMOUNT 4

enum CMD_INPUT {
	DIR_NAME = 1,
	DEPTH = 2, //2
	FILE_NAME = 3 //3
};

/*
FIXIT:
параметр _depth выглядит мутно.
представьте, что вы хотите воспользоваться этой ф-ей. что подставить в качестве последнего параметра? 
вот и я не знаю. вы подставляете 0.

обычно в таких случаях делают так: создают FindFileImpl с 4мя параметрами.

а ещё FindFile c тремя понятными параметрами, которая внутри себя вызывает FindFileImpl и подставляет
магический 0 ... "пользователя" не нужно посвещать в детали вашей реализации ...
*/
int FindFile (char* dir_name, size_t depth, char* file_name, size_t _depth) {
<<<<<<< HEAD
	bool ignore = false;
=======
	/*
	зачем слово static тут?
	*/
	static bool ignore = false;
	
	/*
	почему нельзя из depth было вычитать 1?
	и остановиться, когда она станет равной 0 ... без ввода дополнительной переменной
	*/
>>>>>>> eb72d1cb744f101e2a0e9c4b173b7b0d594c3354
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
<<<<<<< HEAD
		ignore = !strcmp(dir_rec->d_name, ".") || !strcmp(dir_rec->d_name, "..");
=======
		/*
		fixit: пробелы вокруг бинарных операторов
		*/
		if (!strcmp(dir_rec->d_name, ".")||!strcmp(dir_rec->d_name, "..")) {
			ignore = true;
		} else {
			ignore = false;
		}
		/*
		можно было бы просто ignore = !strcmp(dir_rec->d_name, ".") || !strcmp(dir_rec->d_name, "..");
		без 5 строк кода
		*/
>>>>>>> eb72d1cb744f101e2a0e9c4b173b7b0d594c3354
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
	if (argc != PARAMAMOUNT) {
		printf("Incorrect amount of cmd params.\n");
		return 1;
	}
	if (!argv[DIR_NAME] || !argv[DEPTH] || !argv[FILE_NAME]) {
		printf("Incorrect input.\n");
		return 1;
	}
	if (!isdigit(argv[DEPTH])) {
		printf("Incorrect depth.\n");
		return 1;
	}
	char* file_name = (char*)calloc(MAX_STR_LEN, sizeof(char));
<<<<<<< HEAD
        char* dir_name = (char*)calloc(MAX_STR_LEN, sizeof(char));
	size_t depth = atoi(argv[DEPTH]);
	strcpy(file_name, argv[FILE_NAME]);
	strcpy(dir_name, argv[DIR_NAME]);
=======
	char* dir_name = (char*)calloc(MAX_STR_LEN, sizeof(char));
	/*
	По-хорошему сюда стоит добавить проверку количества входных паратеров.
	*/
	size_t depth = atoi(argv[2]);
	/*
	А также проверить, получилось ли сконвертировать строку в число.
	*/
	strcpy(file_name, argv[3]);
	strcpy(dir_name, argv[1]);
>>>>>>> eb72d1cb744f101e2a0e9c4b173b7b0d594c3354
	if (FindFile(dir_name, depth, file_name, 0)) {
		printf("File not found\n");
	}
	free(file_name);
	free(dir_name);
	return 0;
}	
	

