#include "s21_grep.h"

int main(int argc, char** argv) {
  struct flags use_flag = {};
  int flag_error = 0;
  if (!parse_string(argc, argv, &use_flag)) {
    int ind_pattern = optind;
    int ind_file = optind + 1;
    print_file(argv, &use_flag, &ind_pattern, &ind_file, &argc);
  }
  return flag_error;
}

int parse_string(int argc, char** argv, struct flags* use_flag) {
  int opt, flag_error = 0;
  while ((opt = getopt_long(argc, argv, "+eivclnhsfo", NULL, NULL)) != -1 &&
         !flag_error) {
    switch (opt) {
      case 'e':
        use_flag->flag_e = 1;
        break;
      case 'i':
        use_flag->flag_i = 1;
        break;
      case 'v':
        use_flag->flag_v = 1;
        break;
      case 'c':
        use_flag->flag_c = 1;
        break;
      case 'l':
        use_flag->flag_l = 1;
        break;
      case 'n':
        use_flag->flag_n = 1;
        break;
      case '?':
        opterr = 0;
        flag_error = 1;
        break;
    }
  }
  if (argc < optind + 2) {
    printf("No arguments\n");
    flag_error = 1;
  }
  return flag_error;
}

void print_file(char** argv, struct flags* use_flag, const int* ind_pattern,
                const int* ind_file, const int* argc) {
  for (int i = *ind_file; i < *argc; i++) {
    FILE* file_stream = fopen(argv[i], "r");
    if (!file_stream) {
      printf("./s21_grep: %s: No such file or directory\n", argv[i]);
    } else {
      process_file(file_stream, argv[i], use_flag, argv[*ind_pattern]);
      fclose(file_stream);
    }
  }
}

int compile_regex(regex_t* regex, const char* pattern, int case_insensitive) {
  int flags = REG_EXTENDED;
  if (case_insensitive) flags |= REG_ICASE;
  int ret = regcomp(regex, pattern, flags);
  if (ret != 0) {
    char errbuf[100];
    regerror(ret, regex, errbuf, sizeof(errbuf));
    printf("Error: %s\n", errbuf);
  }
  return ret;
}

void process_file(FILE* file_stream, const char* file_name,
                  struct flags* use_flag, const char* pattern) {
  regex_t regex;
  if (compile_regex(&regex, pattern, use_flag->flag_i) != 0) return;
  int count_str = 0, count_find_str = 0;
  char* buffer_ptr = NULL;
  size_t len = 0;
  int flag_break = 0;
  while (getline(&buffer_ptr, &len, file_stream) != -1 && !flag_break) {
    char* ptr_end_string;
    if ((ptr_end_string = strrchr(buffer_ptr, '\n')) != NULL)
      *ptr_end_string = '\0';
    int match = regexec(&regex, buffer_ptr, 0, NULL, 0);
    count_str++;
    if (match == 0) count_find_str++;
    if (use_flag->flag_l && !match) {
      printf("%s\n", file_name);
      flag_break = 1;
    }
    if (!match && (use_flag->flag_e || use_flag->flag_i)) puts(buffer_ptr);
    if (!match && use_flag->flag_n) {
      printf("%d:", count_str);
      puts(buffer_ptr);
    }
    if (use_flag->flag_v && match) puts(buffer_ptr);
  }
  if (use_flag->flag_c) printf("%d\n", count_find_str);
  free(buffer_ptr);
  regfree(&regex);
  return;
}