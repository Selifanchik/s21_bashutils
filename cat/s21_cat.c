#include "s21_cat.h"

int main(int argc, char** argv) {
  CatFlags flags = {};
  int file_count = 0;
  int* file_index = (int*)calloc(argc - 1, sizeof(int));
  if (file_index == 0) {
    printf("Memory allocation error");
    return 1;
  }
  if (parse_string(argc, argv, &flags, file_index, &file_count) == 0) {
    open_file(argv, &flags, file_index, file_count);
  }
  free(file_index);
  return 0;
}

int parse_string(int argc, char** argv, CatFlags* flags, int* file_index,
                 int* file_count) {
  int opt, flag_error = 0;
  while ((opt = getopt_long(argc, argv, "-setETbn", long_options, NULL)) !=
             -1 &&
         !flag_error) {
    switch (opt) {
      case 's':
        flags->flag_s = 1;
        break;
      case 'e':
        flags->flag_e = 1;
        flags->flag_v = 1;
        break;
      case 'E':
        flags->flag_e = 1;
        break;
      case 't':
        flags->flag_t = 1;
        flags->flag_v = 1;
        break;
      case 'T':
        flags->flag_t = 1;
        break;
      case 'b':
        flags->flag_b = 1;
        flags->flag_n = 0;
        break;
      case 'n':
        if (flags->flag_b == 0) {
          flags->flag_n = 1;
        }
        break;
      case '?':
        opterr = 0;
        flag_error = 1;
        break;
      case 1:
        file_index[(*file_count)++] = optind - 1;
        break;
      default:
        printf("Unknown error");
        flag_error = 1;
    }
  }
  if (*file_count == 0 && !flag_error) {
    printf("No input files specified");
    flag_error = 1;
  }
  return flag_error;
}

void open_file(char** file_name, CatFlags* flags, const int* file_index,
               int file_count) {
  int count_number = 0, count_empty_line = 0;
  char prev_char = '\n';
  for (int i = 0; i < file_count; i++) {
    int index = file_index[i];
    FILE* file_stream = fopen(file_name[index], "r");
    if (!file_stream) {
      printf("./s21_cat: %s: No such file or directory\n", file_name[index]);
    } else {
      process_file(file_stream, flags, &count_number, &prev_char,
                   &count_empty_line);
      fclose(file_stream);
    }
  }
}

void process_file(FILE* file_stream, CatFlags* flags, int* count_number,
                  char* prev_char, int* count_empty_line) {
  int symbol;
  int print_done;
  while ((symbol = fgetc(file_stream)) != EOF) {
    print_done = 0;
    if (symbol == '\n' && *prev_char == '\n') {
      (*count_empty_line)++;
    } else {
      *count_empty_line = 0;
    }
    if (flags->flag_s && *count_empty_line > 1) continue;
    if (*prev_char == '\n' &&
        (flags->flag_n || (flags->flag_b && *count_empty_line == 0))) {
      printf("%6d\t", ++(*count_number));
    }
    if (flags->flag_e && symbol == '\n') {
      printf("$");
    }
    if (flags->flag_t && symbol == '\t') {
      printf("^I");
      print_done = 1;
    }
    if (flags->flag_v && symbol != '\n' && symbol != '\t' && symbol >= 0 &&
        symbol <= 255) {
      show_nonpriting_char(symbol, &print_done);
    }
    if (!print_done) {
      printf("%c", symbol);
    }
    *prev_char = symbol;
  }
}

void show_nonpriting_char(int symbol, int* print_done) {
  if (symbol >= 128 && symbol <= 255) {
    printf("M-");
    symbol = symbol % 128;
  }
  if (symbol >= 0 && symbol <= 31) {
    printf("^%c", symbol + 64);
  }
  if (symbol >= 32 && symbol <= 126) {
    printf("%c", symbol);
  }
  if (symbol == 127) {
    printf("^?");
  }
  *print_done = 1;
}