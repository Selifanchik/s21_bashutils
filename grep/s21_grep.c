#include "s21_grep.h"

int main(int argc, char** argv) {
  GrepFlags flags = {};
  TypeError error = {};
  int file_ind;
  char pattern[size_pattern] = {};
  if (!parse_string(argc, argv, &flags, pattern, &error)) {
    if (is_empty(pattern)) {
      strcat(pattern, argv[optind]);
      file_ind = optind + 1;
    } else
      file_ind = optind;
    if (argc <= file_ind)
      printf(
          "There are not enough parameters\n Usage: grep [OPTION] PATTERNS "
          "[FILE]\n");
    else
      open_file(&argc, argv, &flags, pattern, &file_ind);
  }
  return 0;
}

int parse_string(int argc, char** argv, GrepFlags* flags, char* pattern,
                 TypeError* error) {
  int opt, flag_error = 0;
  while ((opt = getopt_long(argc, argv, "+e:ivclnhsf:o", NULL, NULL)) != -1 &&
         !flag_error) {
    switch (opt) {
      case 'e':
        append_pattern(pattern, optarg);
        flags->flag_e = 1;
        break;
      case 'i':
        flags->flag_i = 1;
        break;
      case 'v':
        flags->flag_v = 1;
        break;
      case 'c':
        flags->flag_c = 1;
        break;
      case 'l':
        flags->flag_l = 1;
        break;
      case 'n':
        flags->flag_n = 1;
        break;
      case 'h':
        flags->flag_h = 1;
        break;
      case 's':
        flags->flag_s = 1;
        break;
      case 'f':
        open_pattern(pattern, optarg, error);
        if (error->error) flag_error = 1;
        flags->flag_f = 1;
        break;
      case 'o':
        flags->flag_o = 1;
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

void open_pattern(char* pattern, const char* file_pattern, TypeError* error) {
  FILE* fp = fopen(file_pattern, "r");
  if (!fp) {
    printf("./s21_grep: %s: No such file or directory\n", file_pattern);
    error->error = 1;
  } else {
    char pattern_buffer[size_line] = {};
    while (fgets(pattern_buffer, sizeof(pattern_buffer), fp)) {
      remove_newline(pattern_buffer);
      append_pattern(pattern, pattern_buffer);
    }
    fclose(fp);
  }
}

void open_file(const int* argc, char** argv, GrepFlags* flags,
               const char* pattern, const int* file_ind) {
  if (*argc - *file_ind < 2) flags->flag_h = 1;
  for (int i = *file_ind; i < *argc; i++) {
    FILE* file_stream = fopen(argv[i], "r");
    if (!file_stream) {
      if (!flags->flag_s)
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
    remove_newline(line);
    process_line(line, ++count_str, &regex, flags, &count_find_str, file_name,
                 &flag_break);
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

  if (flags->flag_l && match == 0) {
    printf("%s\n", file_name);
    *break_flag = 1;
    return;
  }

  if (flags->flag_v && match != 0) {
    if (!flags->flag_h) printf("%s:", file_name);
    puts(line_ptr);
    return;
  }

  if (match == 0 && !flags->flag_v && !flags->flag_c && !flags->flag_o) {
    if (!flags->flag_h) printf("%s:", file_name);
    if (flags->flag_n)
      printf("%d:%s\n", line_num, line_ptr);
    else
      puts(line_ptr);
  }
  if (flags->flag_o) print_only_matches(line_ptr, regex);
}

void print_only_matches(const char* buffer, regex_t* regex) {
  const char* start_find = buffer;
  regmatch_t match;
  while (regexec(regex, start_find, 1, &match, 0) == 0) {
    printf("%.*s\n", match.rm_eo - match.rm_so, start_find + match.rm_so);
    start_find += match.rm_eo;
  }
}

int is_empty(const char* pattern) { return (pattern[0] == '\0') ? 1 : 0; }

void remove_newline(char* str) {
  char* ptr_end = strrchr(str, '\n');
  if (ptr_end) *ptr_end = '\0';
}

void append_pattern(char* pattern, const char* new_part) {
  if (!is_empty(pattern)) {
    strcat(pattern, "|");
  }
  strcat(pattern, new_part);
}
