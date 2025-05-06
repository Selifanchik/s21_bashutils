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
  int flag_h;
  int flag_s;
  int flag_f;
  int flag_o;
};

int parse_string(int argc, char** argv, struct flags* use_flag);
void print_file(char** file_name, struct flags* use_flag,
                const int* ind_pattern, const int* ind_file, const int* argc);
void process_file(FILE* file_stream, const char* file_name,
                  struct flags* use_flag, const char* pattern);
// void print_match(const char* buffer, regex_t* regex);
FILE* file_open(const char* file_name, struct flags* use_flag);

#endif