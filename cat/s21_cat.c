#include "s21_cat.h"


int main(int argc, char** argv) {
    int i;
    if (argc > 2 && argv[1][0] == '-') {
        parse_flag(argc, argv);
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
        printf("cat: %s: No such file or directory", file_name);
        return;
    }
    while (fgets(buff, sizeof(buff), fp)) {
        printf("%s", buff);
    }
    fclose(fp);
    return;
}

void parse_flag(int argc, const char** argv) {
    char* use_flag = (char*)malloc(10 * sizeof(char*));
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') append_flag(argv[i], use_flag);
    }
    puts(use_flag);
}

void append_flag(const char* str, char* use_flag) {
    for (int i = 0; i < count_flag; i++) {
        if (!strcmp(str, opt[i].flags)) 
            strcat(use_flag, opt[i].simple_flag);
    }
}
