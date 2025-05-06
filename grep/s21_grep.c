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
  while ((opt = getopt_long(argc, argv, "+eivcln", NULL, NULL)) != -1 &&
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
    printf("No arguments");
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

void process_file(FILE* file_stream, const char* file_name,
                  struct flags* use_flag, const char* pattern) {
  regex_t regex;
  int match, count_str = 0, count_find_str = 0;
  if (use_flag->flag_i)
    match = regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE);
  else
    match = regcomp(&regex, pattern, REG_EXTENDED);
  if (match != 0) {
    char errbuf[100];
    regerror(match, &regex, errbuf, sizeof(errbuf));
    printf("Error: %s\n", errbuf);
    return;
  }
  char* buffer_ptr = NULL;
  size_t len = 0;
  int flag_break = 0;
  while (getline(&buffer_ptr, &len, file_stream) != -1 && !flag_break) {
    char* ptr_end_string;
    if ((ptr_end_string = strrchr(buffer_ptr, '\n')) != NULL)
      *ptr_end_string = '\0';
    int reg = regexec(&regex, buffer_ptr, 0, NULL, 0);
    count_str++;
    if (reg == 0) count_find_str++;
    if (use_flag->flag_l && !reg) {
      printf("%s\n", file_name);
      flag_break = 1;
    }
    if (use_flag->flag_e || use_flag->flag_i) print_match(buffer_ptr, &regex);
    if (use_flag->flag_n) {
      if (reg == 0) printf("%d:", count_str);
      print_match(buffer_ptr, &regex);
    }
    if (use_flag->flag_v && (reg != 0)) printf("%s\n", buffer_ptr);
  }
  if (use_flag->flag_c) printf("%d\n", count_find_str);
  free(buffer_ptr);
  regfree(&regex);
  return;
}

void print_match(const char* buffer, regex_t* regex) {
  int print_done = 0;
  const char* start_find = buffer;
  regmatch_t match;
  while (regexec(regex, start_find, 1, &match, 0) == 0) {
    print_done = 1;
    printf("%.*s", match.rm_so, start_find);
    printf("%.*s", match.rm_eo - match.rm_so, start_find + match.rm_so);
    start_find += match.rm_eo;
  }
  if (print_done) printf("%s\n", start_find);
}