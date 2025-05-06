#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flags {
  int flag_e;
  int flag_i;
  int flag_v;
  int flag_c;
  int flag_l;
  int flag_n;
};

int parse_string(int argc, char** argv, struct flags* use_flag);
void print_file(char** file_name, struct flags* use_flag,
                const int* ind_pattern, const int* ind_file, const int* argc);
void process_file(FILE* file_stream, const char* file_name,
                  struct flags* use_flag, const char* pattern);
int compile_regex(regex_t* regex, const char* pattern, int case_insensitive);
void process_line(const char* line_ptr, int line_num, regex_t* regex,
  struct flags* use_flag, int* count_find_str,
  const char* file_name, int* break_flag);
#endif