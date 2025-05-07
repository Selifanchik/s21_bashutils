#include "s21_grep.h"

int main(int argc, char** argv) {
  GrepFlags flags = {};
  int file_ind;
  char pattern[1024] = {};
  if (!parse_string(argc, argv, &flags, pattern)) {
    if (pattern[0] == '\0') { 
      strcat(pattern, argv[optind]);
      file_ind = optind + 1;
    } else file_ind = optind;
    if (argc <= file_ind) printf("There are not enough parameters\n Usage: grep [OPTION] PATTERNS [FILE]\n");
    else print_file(&argc, argv, &flags, pattern, &file_ind);
  }
  return 0;
}

int parse_string(int argc, char** argv, GrepFlags* flags, char* pattern) {
  int opt, flag_error = 0;
  while ((opt = getopt_long(argc, argv, "+e:ivclnh", NULL, NULL)) != -1 &&
         !flag_error) {
    switch (opt) {
      case 'e':
        if (!flags->flag_e) {
          flags->flag_e = 1;
          strcat(pattern, optarg);
        } else {
          strcat(pattern, "|\0");
          strcat(pattern, optarg);
        }
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
      case 'h':
        flags->flag_h = 1;
        break;
      case '?':
        opterr = 0;
        flag_error = 1;
        break;
    }
  }
  GrepFlags zero = {};
  if (!memcmp(flags, &zero, sizeof(GrepFlags))) flags->flag_e = 1;
  return flag_error;
}

void print_file(const int* argc, char** argv, GrepFlags* flags, const char* pattern,
                const int* file_ind) {
  if (*argc - *file_ind < 2) flags->flag_h = 1;
  for (int i = *file_ind; i < *argc; i++) {
    FILE* file_stream = fopen(argv[i], "r");
    if (!file_stream) {
      printf("./s21_grep: %s: No such file or directory\n", argv[i]);
    } else {
      process_file(file_stream, argv[i], flags, pattern);
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
  if (flags->flag_c) {
    if (!flags->flag_h) printf("%s:", file_name);
    printf("%d\n", count_find_str);
  }
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
  if (!match && (flags->flag_e || flags->flag_i || flags->flag_h)) {
    if (!flags->flag_h) printf("%s:", file_name);
    puts(line_ptr);
  }
  if (!match && flags->flag_n) {
    if (!flags->flag_h) printf("%s:", file_name);
    printf("%d:%s\n", line_num, line_ptr);
  }
  if (flags->flag_v && match) {
    if (!flags->flag_h) printf("%s:", file_name);
    puts(line_ptr);
  }
}
