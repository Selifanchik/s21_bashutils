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
  while ((opt = getopt_long(argc, argv, "+eivcln", NULL, NULL)) != -1 && !flag_error) {
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
  if (argc < optind + 2)  {
    printf("No arguments");
    flag_error = 1;
  }
  return flag_error;
}

void print_file(char** file_name, struct flags* use_flag, const int* ind, const int* argc){
  for (int i = *ind; i < *argc; i++) {
    FILE* file_stream = fopen(file_name[i], "r");
    if (!file_stream) {
      printf("./s21_cat: %s: No such file or directory\n", file_name[i]);
    } else {
      process_file(file_stream, use_flag);
      fclose(file_stream);
    }
  }
}

void process_file(FILE* file_stream, struct flags* use_flag) {
  char buffer[4096];
  regex_t regex;
  char* pattern = "a+b";
  int ret = regcomp(&regex, pattern, REG_EXTENDED);
  if (ret != 0) {
    char errbuf[100];
    regerror(ret, &regex, errbuf, sizeof(errbuf));
    printf("Error: %s\n", errbuf);
    return;
  }
  while(fgets(buffer, sizeof(buffer), file_stream)) {
    const char* start_find = buffer;
    regmatch_t match;
    while(regexec(&regex, start_find, 1, &match, 0) == 0) {
      puts(start_find);
      puts("\n");
      // printf("%d\n ", match.rm_so);
      printf("%.*s", match.rm_so, start_find);
      start_find += match.rm_eo + 1;
    }
  }
  regfree(&regex);
  return ;
}