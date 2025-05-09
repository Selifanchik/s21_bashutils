all: format s21_grep

s21_grep: s21_grep.o utils.o
	gcc -o s21_grep s21_grep.o utils.o

s21_grep.o: s21_grep.c
	gcc -Wall -Werror -Wextra -std=c11 -g -c s21_grep.c -o s21_grep.o

utils.o: utils.c
	gcc -Wall -Werror -Wextra -std=c11 -g -c utils.c -o utils.o

format:
	clang-format -i -style=Google *.c
	clang-format -i -style=Google *.h


clean:
	rm -f s21_grep.o s21_grep

rebuild: clean all
