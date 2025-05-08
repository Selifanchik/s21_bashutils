#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

enum { size_line = 4096 };

typedef struct {
  int flag_e;
  int flag_i;
  int flag_v;
  int flag_c;
  int flag_l;
  int flag_n;
  int flag_h;
  int flag_s;
  int flag_f;
  int flag_o;
} GrepFlags;

typedef struct {
  int error;
} TypeError;

int parse_string(int argc, char** argv, GrepFlags* flags, char* pattern,
                 TypeError* error);
void print_file(const int* argc, char** file_name, GrepFlags* flags,
                const char* pattern, const int* ind_file);
void process_file(FILE* file_stream, const char* file_name, GrepFlags* flags,
                  const char* pattern);
int compile_regex(regex_t* regex, const char* pattern, int flag_i);
void process_line(const char* line_ptr, int line_num, regex_t* regex,
                  GrepFlags* flags, int* count_find_str, const char* file_name,
                  int* break_flag);
void open_pattern(char* pattern, const char* file_pattern, GrepFlags* flags,
                  TypeError* error);
void print_only_matches(const char* pattern, regex_t* regex);
#endif