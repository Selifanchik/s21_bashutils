#include "s21_cat.h"


int main(int argc, char** argv) {
    int i;
    if (argc > 2 && argv[1][0] == '-') {
        i = 2;
    } else i = 1;
    for (; i < argc; i++) {
        print_file(argv[i]);
    }
    return 0;
}

void print_file(const char* file_name) {
    char buff[100] = {};
    FILE* fp = fopen(file_name, "r");
    if (fp == NULL) {
        perror("Ошибка открытия файла");
        return;
    }
    while (fgets(buff, sizeof(buff), fp)) {
        printf("%s", buff);
    }
    fclose(fp);
    return;
}
