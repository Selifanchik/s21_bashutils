#include "s21_grep.h"

int main(int argc, char** argv) {
  GrepFlags flags = {};
  int flag_error = 0;
  if (!parse_string(argc, argv, &flags)) {
    int ind_pattern = optind;
    int ind_file = optind + 1;
    print_file(argv, &flags, &ind_pattern, &ind_file, &argc);
  }
  return flag_error;
}

int parse_string(int argc, char** argv, GrepFlags* flags) {
  int opt, flag_error = 0;
  while ((opt = getopt_long(argc, argv, "+eivcln", NULL, NULL)) != -1 &&
         !flag_error) {
    switch (opt) {
      case 'e':
        flags->flag_e = 1;
        break;
      case 'i':
        if (!flags->flag_c && !flags->flag_l) flags->flag_i = 1;
        break;
      case 'v':
        if (!flags->flag_c && !flags->flag_l) flags->flag_v = 1;
        break;
      case 'c':
        if (!flags->flag_l) {
          memset(flags, 0, sizeof(*flags));
          flags->flag_c = 1;
        }
        break;
      case 'l':
        memset(flags, 0, sizeof(*flags));
        flags->flag_l = 1;
        break;
      case 'n':
        flags->flag_n = 1;
        break;
      case '?':
        opterr = 0;
        flag_error = 1;
        break;
    }
  }
  GrepFlags zero = {};
  if (!memcmp(flags, &zero, sizeof(GrepFlags))) flags->flag_e = 1;
  if (argc < optind + 2) {
    printf("No arguments\n");
    flag_error = 1;
  }
  return flag_error;
}

void print_file(char** argv, GrepFlags* flags, const int* ind_pattern,
                const int* ind_file, const int* argc) {
  for (int i = *ind_file; i < *argc; i++) {
    FILE* file_stream = fopen(argv[i], "r");
    if (!file_stream) {
      printf("./s21_grep: %s: No such file or directory\n", argv[i]);
    } else {
      process_file(file_stream, argv[i], flags, argv[*ind_pattern]);
      fclose(file_stream);
    }
  }
}

void process_file(FILE* file_stream, const char* file_name, GrepFlags* flags,
                  const char* pattern) {
  regex_t regex;
  if (compile_regex(&regex, pattern, flags->flag_i) != 0) return;
  int count_str = 0, count_find_str = 0, flag_break = 0;
  char line[size_line] = {};
  while (fgets(line, sizeof(line), file_stream) && !flag_break) {
    char* ptr_end_string = strrchr(line, '\n');
    if (ptr_end_string != NULL) *ptr_end_string = '\0';
    process_line(line, ++count_str, &regex, flags, &count_find_str,
                 file_name, &flag_break);
  }
  if (flags->flag_c) printf("%d\n", count_find_str);
  regfree(&regex);
  return;
}

int compile_regex(regex_t* regex, const char* pattern, int flag_i) {
  int flags = REG_EXTENDED;
  if (flag_i) flags |= REG_ICASE;
  int ret = regcomp(regex, pattern, flags);
  if (ret != 0) {
    char errbuf[100];
    regerror(ret, regex, errbuf, sizeof(errbuf));
    printf("Error: %s\n", errbuf);
  }
  return ret;
}

void process_line(const char* line_ptr, int line_num, regex_t* regex,
                  GrepFlags* flags, int* count_find_str, const char* file_name,
                  int* break_flag) {
  int match = regexec(regex, line_ptr, 0, NULL, 0);
  if (match == 0) (*count_find_str)++;
  if (flags->flag_l && !match) {
    printf("%s\n", file_name);
    *break_flag = 1;
    return;
  }
  if (!match && (flags->flag_e || flags->flag_i)) {
    puts(line_ptr);
  }
  if (!match && flags->flag_n) {
    printf("%d:%s\n", line_num, line_ptr);
  }
  if (flags->flag_v && match) {
    puts(line_ptr);
  }
}
