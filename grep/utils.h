#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>

int is_empty(const char* pattern);
void remove_newline(char* str);
void append_pattern(char* pattern, const char* new_part);
FILE* open_file(const char* file_name, const int* flag_s);

#endif
