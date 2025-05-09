#include "s21_grep.h"

#include "utils.h"

int main(int argc, char** argv) {
  GrepFlags flags = {};
  int file_ind;
  char pattern[SIZE_PATTERN] = {};
  if (!parse_string(argc, argv, &flags, pattern)) {
    if (is_empty(pattern)) {
      strcat(pattern, argv[optind]);
      file_ind = optind + 1;
    } else {
      file_ind = optind;
    }
    if (argc <= file_ind) {
      printf("Usage: grep [OPTION] PATTERNS [FILE]\n");
    } else if (!(flags.flag_o && flags.flag_v)) {
      apply_grep_to_files(argc, argv, &flags, pattern, &file_ind);
    }
  }
  return 0;
}

int parse_string(int argc, char** argv, GrepFlags* flags, char* pattern) {
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
        if (open_pattern(pattern, optarg)) {
          flag_error = 1;
        }
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
  return flag_error;
}

int open_pattern(char* pattern, const char* file_pattern) {
  int flag_s = 0, flag_error = 0;
  FILE* file_stream = open_file(file_pattern, &flag_s);
  if (file_stream) {
    char pattern_buffer[SIZE_LINE] = {};
    while (fgets(pattern_buffer, sizeof(pattern_buffer), file_stream)) {
      remove_newline(pattern_buffer);
      append_pattern(pattern, pattern_buffer);
    }
    fclose(file_stream);
  } else {
    flag_error = 1;
  }
  return flag_error;
}

void apply_grep_to_files(int argc, char** argv, GrepFlags* flags,
                         const char* pattern, const int* file_ind) {
  if (argc - *file_ind < 2) {
    flags->flag_h = 1;
  }
  for (int i = *file_ind; i < argc; i++) {
    FILE* file_stream = open_file(argv[i], &flags->flag_s);
    if (file_stream) {
      process_file(file_stream, argv[i], flags, pattern);
      fclose(file_stream);
    }
  }
}

void process_file(FILE* file_stream, const char* file_name, GrepFlags* flags,
                  const char* pattern) {
  regex_t regex;
  if (compile_regex(&regex, pattern, flags->flag_i) != 0) return;
  int line_count = 0, selected_count = 0;
  flags->break_flag = 0;
  char line[SIZE_LINE] = {};
  while (fgets(line, sizeof(line), file_stream) && !flags->break_flag) {
    remove_newline(line);
    process_line(line, ++line_count, &regex, flags, &selected_count, file_name);
  }
  if (flags->flag_c && !flags->flag_l) {
    if (!flags->flag_h) {
      printf("%s:", file_name);
    }
    printf("%d\n", selected_count);
  }
  regfree(&regex);
}

int compile_regex(regex_t* regex, const char* pattern, int flag_i) {
  int flags = REG_EXTENDED;
  if (flag_i) {
    flags |= REG_ICASE;
  }
  int ret = regcomp(regex, pattern, flags);
  if (ret != 0) {
    char errbuf[100];
    regerror(ret, regex, errbuf, sizeof(errbuf));
    printf("Error: %s\n", errbuf);
  }
  return ret;
}

void process_line(const char* line_ptr, int line_count, regex_t* regex,
                  GrepFlags* flags, int* selected_count,
                  const char* file_name) {
  int match = regexec(regex, line_ptr, 0, NULL, 0);
  int is_match = (match == 0);
  if (flags->flag_l &&
      ((is_match && !flags->flag_v) || (!is_match && flags->flag_v))) {
    printf("%s\n", file_name);
    flags->break_flag = 1;
  } else if (flags->flag_c &&
             ((is_match && !flags->flag_v) || (!is_match && flags->flag_v))) {
    (*selected_count)++;
  } else if (flags->flag_o) {
    print_only_matches(line_ptr, regex, flags, file_name, line_count);
  } else if (flags->flag_v && !is_match) {
    print_name_and_number(flags, file_name, line_count);
    puts(line_ptr);
  } else if (is_match && !flags->flag_v) {
    print_name_and_number(flags, file_name, line_count);
    puts(line_ptr);
  }
}

void print_only_matches(const char* buffer, regex_t* regex,
                        const GrepFlags* flags, const char* file_name,
                        int line_count) {
  const char* start_find = buffer;
  regmatch_t match;
  while (regexec(regex, start_find, 1, &match, 0) == 0) {
    print_name_and_number(flags, file_name, line_count);
    printf("%.*s\n", match.rm_eo - match.rm_so, start_find + match.rm_so);
    start_find += (match.rm_so == match.rm_eo) ? 1 : match.rm_eo;
  }
}

void print_name_and_number(const GrepFlags* flags, const char* file_name,
                           int line_count) {
  if (!flags->flag_h) {
    printf("%s:", file_name);
  }
  if (flags->flag_n) {
    printf("%d:", line_count);
  }
}