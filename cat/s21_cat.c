#include "s21_cat.h"

int main(int argc, char** argv) {
  CATFLAGS use_flag = {};
  int flag_error = 0;
  int* arg_ind = (int*)calloc(argc - 1, sizeof(int));
  int ind = 0;
  if (arg_ind == 0) {
    flag_error = 1;
    printf("Memory allocation error");
  }
  if (parse_string(argc, argv, &use_flag, arg_ind, &ind) == 0)
    print_file(argv, &use_flag, arg_ind, &ind);
  free(arg_ind);
  return flag_error;
}

int parse_string(int argc, char** argv, CATFLAGS* use_flag, int* arg_index,
                 int* ind) {
  int opt, flag_error = 0;
  while ((opt = getopt_long(argc, argv, "-setETbn", long_options, NULL)) !=
             -1 &&
         !flag_error) {
    switch (opt) {
      case 's':
        use_flag->flag_s = 1;
        break;
      case 'e':
        use_flag->flag_e = 1;
        use_flag->flag_v = 1;
        break;
      case 'E':
        use_flag->flag_e = 1;
        break;
      case 't':
        use_flag->flag_t = 1;
        use_flag->flag_v = 1;
        break;
      case 'T':
        use_flag->flag_t = 1;
        break;
      case 'b':
        use_flag->flag_b = 1;
        use_flag->flag_n = 0;
        break;
      case 'n':
        if (use_flag->flag_b == 0) use_flag->flag_n = 1;
        break;
      case '?':
        opterr = 0;
        flag_error = 1;
        break;
      default:
        if (optarg)
          arg_index[(*ind)++] = optind - 1;
        else {
          printf("Unknown error");
          flag_error = 1;
        }
        break;
    }
  }
  if (*ind == 0 && !flag_error) {
    printf("No arguments");
    flag_error = 1;
  }
  return flag_error;
}

void print_file(char** file_name, CATFLAGS* use_flag, const int* arg_ind,
                const int* ind) {
  int count_number = 0;
  int count_empty_line = 0;
  char previous_symbol = '\n';
  for (int i = 0; i < *ind; i++) {
    int index = arg_ind[i];
    FILE* file_stream = fopen(file_name[index], "r");
    if (!file_stream) {
      printf("./s21_cat: %s: No such file or directory\n", file_name[index]);
    } else {
      process_file(file_stream, use_flag, &count_number, &previous_symbol,
                   &count_empty_line);
      fclose(file_stream);
    }
  }
}

void process_file(FILE* file_stream, CATFLAGS* use_flag, int* count_number,
                  char* previous_symbol, int* count_empty_line) {
  int symbol;
  int print_done;
  while ((symbol = fgetc(file_stream)) != EOF) {
    print_done = 0;
    if (symbol == '\n' && *previous_symbol == '\n')
      (*count_empty_line)++;
    else
      *count_empty_line = 0;
    if (use_flag->flag_s && *count_empty_line > 1) continue;
    if (*previous_symbol == '\n' &&
        (use_flag->flag_n || (use_flag->flag_b && *count_empty_line == 0)))
      printf("%6d\t", ++(*count_number));
    if (use_flag->flag_e && symbol == '\n') printf("$");
    if (use_flag->flag_t && symbol == '\t') {
      printf("^I");
      print_done = 1;
    }
    if (use_flag->flag_v && symbol != '\n' && symbol != '\t' && symbol >= 0 &&
        symbol <= 255)
      show_nonpriting_char(&symbol, &print_done);
    if (!print_done) printf("%c", symbol);
    *previous_symbol = symbol;
  }
}

void show_nonpriting_char(const int* symbol, int* print_done) {
  int new_symbol;
  if (*symbol >= 128 && *symbol <= 255) {
    printf("M-");
    new_symbol = *symbol % 128;
  } else
    new_symbol = *symbol;
  if (new_symbol >= 0 && new_symbol <= 31) printf("^%c", new_symbol + 64);
  if (new_symbol >= 32 && new_symbol <= 126) printf("%c", new_symbol);
  if (new_symbol == 127) printf("^?");
  *print_done = 1;
}