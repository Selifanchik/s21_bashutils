#include "s21_cat.h"

int main(int argc, char** argv) {
    struct flags use_flag = {};
    int flag_error = 0;
    int* arg_ind = (int*)calloc(argc - 1, sizeof(int));
    int ind = 0;
    if (arg_ind == 0) {
        flag_error = 1;
        printf("Memory allocation error");
    }
    if (parse_string(argc, argv, &use_flag, arg_ind, &ind) == 0) {
        int index;
        for (int i = 0; i < ind; i++) {
            index = arg_ind[i];
            print_file(argv[index], &use_flag);
        }
    }
    free(arg_ind);
    return flag_error;
}

int parse_string(int argc, char** argv, struct flags* use_flag, int* arg_index, int* ind) {
    int opt, flag_error = 0;
    while ((opt = getopt_long(argc, argv, "-setETbn", long_options, NULL)) != -1 && !flag_error) {
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
            flag_error = 1;
            break;
        default:
            if (optarg) arg_index[(*ind)++] = optind - 1;
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

void print_file(const char* file_name, struct flags* use_flag) {
    FILE* file_stream = fopen(file_name, "r");
    if (!file_stream) {
        printf("./s21_cat: %s: No such file or directory", file_name);
    } else process_file(file_stream, use_flag);
}


void process_file(FILE* file_stream, struct flags* use_flag) {
    int symbol;
    int count_number = 0, count_empty_line = 0;
    char previous_symbol = '\n';
    int print_done = 0;
    while ((symbol = fgetc(file_stream)) != EOF) {
        if (symbol == '\n' && previous_symbol == '\n') count_empty_line++;
        else count_empty_line = 0;
        if (use_flag->flag_s) 
            if (count_empty_line > 1) continue;
        if (previous_symbol == '\n' && (use_flag->flag_n || (use_flag->flag_b && count_empty_line == 0))) 
            printf("%6d\t", ++count_number);
        if (use_flag->flag_e) 
            if (symbol == '\n') printf("$");
        if (use_flag->flag_t) 
            if (symbol == '\t') {
                printf("^I");
                print_done = 1;
            }
        if (use_flag->flag_v ) 
            if (symbol != '\n' && symbol != '\t')
                process_flag_v(&symbol, &print_done);
        if (!print_done) {
            printf("%c", symbol);
            previous_symbol = symbol;
        }
    }
}

void process_flag_v(const int* symbol, int* print_done) {
    if (*symbol >= 0 && *symbol <= 31) {
        printf("^%c", *symbol + 64);
        *print_done = 1;
    }
    if (*symbol > 127) {
        printf("M-%c", *symbol % 128);
        *print_done = 1;
    }
}