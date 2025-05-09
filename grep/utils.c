#include "utils.h"

int is_empty(const char* pattern) { return (pattern[0] == '\0'); }

void remove_newline(char* str) {
  char* ptr_end = strrchr(str, '\n');
  if (ptr_end) *ptr_end = '\0';
}

void append_pattern(char* pattern, const char* new_part) {
  if (!is_empty(pattern)) {
    strcat(pattern, "|");
  }
  strcat(pattern, new_part);
}

FILE* open_file(const char* file_name, const int* flag_s) {
  FILE* file_stream = fopen(file_name, "r");
  if (!file_stream && !flag_s)
    printf("./s21_grep: %s: No such file or directory\n", file_name);
  return file_stream;
}