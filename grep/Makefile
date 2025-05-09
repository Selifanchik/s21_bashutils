all: format s21_grep

s21_grep: s21_grep.o
	gcc -o s21_grep s21_grep.o

s21_grep.o: s21_grep.c
	gcc -Wall -Werror -Wextra -std=c11 -g -c s21_grep.c -o s21_grep.o

format:
	clang-format -i -style=Google s21_grep.c
	clang-format -i -style=Google s21_grep.h

clean:
	rm -f s21_grep.o s21_grep

rebuild: clean all
