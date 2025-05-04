#ifndef S21_GREP_H
#define S21_GREP_H

#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <regex.h>

struct flags {
	int flag_e;
	int flag_i;
	int flag_v;
	int flag_c;
	int flag_l;
	int flag_n;
  };

  int parse_string(int argc, char** argv, struct flags* use_flag);
  void print_file(char** file_name, struct flags* use_flag, const int* ind, const int* argc);
  void process_file(FILE* file_stream, struct flags* use_flag);

#endif