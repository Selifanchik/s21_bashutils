#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

enum {size_buffer = 256};

struct flags {
  int flag_s;
  int flag_v;
  int flag_e;
  int flag_t;
  int flag_b;
  int flag_n;
};

struct option long_options[] = {
  {"number-nonblank", optional_argument, NULL, 'b'},
  {"number", optional_argument, NULL, 'n'},
  {"squeeze-blank", optional_argument, NULL, 's'},
  {0, 0, 0, 0}
};

int parse_string(int argc, char** argv, struct flags* use_flag, int* arg_index, int* ind);
void print_file(char** file_name, struct flags* use_flag, int* arg_ind, int* ind);
void process_file(FILE* file_stream, struct flags* use_flag, int* count_number);
void process_flag_v(const int* symbol, int* print_done);

#endif
