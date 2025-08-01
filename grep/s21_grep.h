#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

enum { SIZE_LINE = 4096, SIZE_PATTERN = 1096 };

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
  int break_flag;
} GrepFlags;

int parse_string(int argc, char** argv, GrepFlags* flags, char* pattern);
void apply_grep_to_files(int argc, char** file_name, GrepFlags* flags,
                         const char* pattern, const int* ind_file);
void process_file(FILE* file_stream, const char* file_name, GrepFlags* flags,
                  const char* pattern);
int compile_regex(regex_t* regex, const char* pattern, int flag_i);
void process_line(const char* line_ptr, int line_count, regex_t* regex,
                  GrepFlags* flags, int* selected_count, const char* file_name);
int open_pattern(char* pattern, const char* file_pattern);
void print_only_matches(const char* buffer, regex_t* regex,
                        const GrepFlags* flags, const char* file_name,
                        int line_count);
void print_name_and_number(const GrepFlags* flags, const char* file_name,
                           int line_count);

#endif