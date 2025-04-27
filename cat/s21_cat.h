#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_file(const char* file_name);
void parse_flag();
void flag_handler();
void append_flag();

enum {count_flag = 8};

typedef struct {
    char* flags;
    char* simple_flag;
} OPTIONS;

OPTIONS opt[count_flag] = {
    {"-b", "b"}, {"-e", "vE"}, {"-n", "n"}, {"-s", "s"}, {"-t", "vT"},
    {"-number-nonblank", "b"}, {"-number", "n"}, {"-squeezee-blank", "s"}
};

#endif