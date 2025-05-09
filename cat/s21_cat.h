#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

enum { size_buffer = 256 };

typedef struct {
  int flag_s;
  int flag_v;
  int flag_e;
  int flag_t;
  int flag_b;
  int flag_n;
} CatFlags;

struct option long_options[] = {
    {"number-nonblank", optional_argument, NULL, 'b'},
    {"number", optional_argument, NULL, 'n'},
    {"squeeze-blank", optional_argument, NULL, 's'},
    {0, 0, 0, 0}};

int parse_string(int argc, char** argv, CatFlags* flags, int* arg_index,
                 int* ind);
void open_file(char** file_name, CatFlags* flags, const int* arg_ind, int ind);
void process_file(FILE* file_stream, CatFlags* flags, int* count_number,
                  char* previous_symbol, int* count_empty_line);
void show_nonpriting_char(int symbol, int* print_done);

#endif
