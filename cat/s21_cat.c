#include "s21_cat.h"

int main(int argc, char** argv) {
    struct flags use_flag = {};
    int flag_error = 0;
    int* arg_ind = (int*)calloc(argc - 1, sizeof(int));
    int ind = 0;
    if (!arg_ind) {
        flag_error = 1;
        printf("Memory allocation error");
    }
    if (!parse_string(argc, argv, &use_flag, arg_ind, &ind)) {
        printf("yes");
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