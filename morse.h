#ifndef MORSE_INCLUDED

#define MAX_CODE_LENGTH 40
#define SYMBOL_AMOUNT 100
#define MAX_MORSE_MESSAGE 255
#define MAX_MORSE_LEN 1024

const char morse_point = '.';
const char morse_dash = '-';
const char space_char = ' ';

const char* space_str = "#";
const char* sym_delim = "$";

typedef struct {
        char symbol;
        char morse_code[MAX_CODE_LENGTH];
} MorseChar;

int FileSize (FILE* file) {
        fseek(file, SEEK_END, 0);
        int size = ftell(file);
        fseek(file, SEEK_SET, 0);
        return size;
}

void GetMorseTable (FILE* file, MorseChar* morse_table) {
        if (!file) {
                return;
        }
	if (!morse_table) {
                return;
        }
size_t i = 0;
        while (fscanf(file, "<%c>:<%[^>]>\n", &morse_table[i].symbol, morse_table[i].morse_code) != EOF) {
                printf("%c, %s\n", morse_table[i].symbol, morse_table[i].morse_code);
                i++;
        }
}

int WordCounter (char* message) {
        if (!message) return 0;
        int word_counter = 1;
        for (size_t i = 0; i < strlen(message); i++) {
                if (message[i] == space_char) {
                        word_counter++;
                }
        }
        return word_counter;
}

void TranslatorToMorse (char* message, MorseChar* morse_table, char* morse_text) {
        size_t j;
        for (size_t i = 0; i < strlen(message); i++) {
                j = 0;
                if (message[i] == space_char) {
                        strcat(morse_text, space_str);
                        printf("%s, %c\n", morse_text, message[i]);
			i++;
                }
                while (morse_table[j].symbol) {
                        if (message[i] == morse_table[j].symbol) {
                                strcat(morse_text, morse_table[j].morse_code);
                                strcat(morse_text, sym_delim);
                                printf("%s, %c\n", morse_text, message[i]);
                                break;
                        }
                        j++;
                }
        }
}

void SignalExecutor (char* morse_text) {
        for (size_t i = 0; i < strlen(morse_text);i++) {
                switch (morse_text[i]) {
                        case '#':
                                printf("   ");
                                break;
                        case '$':
                                printf(" ");
                                break;
                        case '.':
                                printf("%c", morse_point);
                                break;
                        case '-':
                                printf("%c", morse_dash);
                                break;
                        default:
                                printf("Wromg symbol in morse text.\n");
                                break;
                }
        }
	printf("\n");
}

#define MORSE_INCLUDED
#endif
