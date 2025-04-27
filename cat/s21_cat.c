#include "s21_cat.h"


int main(int argc, const char** argv) {
    return 0;
}

void print_file(const char* file_name) {
    char buff[256] = {};
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

void parse_flag(int argc, const char** argv, char* use_flag, int* ind_arg) {
    int ind = 0;
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') append_flag(argv[i], use_flag);
        else ind_arg[ind++] = i;
    }
}


void append_flag(const char* str, char* use_flag) {
    for (int i = 0; i < count_flag; i++) {
        if (!strcmp(str, opt[i].flags)) 
            strcat(use_flag, opt[i].simple_flag);
    }
}