#include "s21_grep.h"

int main(int argc, char** argv) {
  struct flags use_flag = {};
  int flag_error = 0;
  if (!parse_string(argc, argv, &use_flag)) {
    int ind_arg = optind + 1;
    print_file(argv, &use_flag, &ind_arg, &argc);
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

void print_file(char** file_name, struct flags* use_flag, const int* ind,
                const int* argc) {
  for (int i = *ind; i < *argc; i++) {
    FILE* file_stream = fopen(file_name[i], "r");
    if (!file_stream) {
      printf("./s21_cat: %s: No such file or directory\n", file_name[i]);
    } else {
      process_file(file_stream, file_name[i], use_flag);
      fclose(file_stream);
    }
  }
}

void process_file(FILE* file_stream, const char* file_name,
                  struct flags* use_flag) {
  char buffer[4096];
  regex_t regex;
  char* pattern = "a+b";
  int ret, count_str = 0, count_find_str = 0;
  if (use_flag->flag_i)
    ret = regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE);
  else
    ret = regcomp(&regex, pattern, REG_EXTENDED);
  if (ret != 0) {
    char errbuf[100];
    regerror(ret, &regex, errbuf, sizeof(errbuf));
    printf("Error: %s\n", errbuf);
    return;
  }
  int flag_break = 0;
  while (fgets(buffer, sizeof(buffer), file_stream) && !flag_break) {
    int reg = regexec(&regex, buffer, 0, NULL, 0);
    count_str++;
    if (reg == 0) count_find_str++;
    if (use_flag->flag_l) {
      printf("%s", file_name);
      flag_break = 1;
    }
    if (use_flag->flag_e || use_flag->flag_i) print_find_string(buffer, &regex);
    if (use_flag->flag_n) {
      if (reg == 0) printf("%d:", count_str);
      print_find_string(buffer, &regex);
    }
    if (use_flag->flag_v && (reg != 0)) printf("%s", buffer);
  }
  if (use_flag->flag_c) printf("%d", count_find_str);
  printf("\n");
  regfree(&regex);
  return;
}

void print_find_string(const char* buffer, regex_t* regex) {
  int print_done = 0;
  const char* start_find = buffer;
  regmatch_t match;
  while (regexec(regex, start_find, 1, &match, 0) == 0) {
    print_done = 1;
    printf("%.*s", match.rm_so, start_find);
    printf("%.*s", match.rm_eo - match.rm_so, start_find + match.rm_so);
    start_find += match.rm_eo + 1;
  }
  if (print_done) printf("%s", start_find);
}