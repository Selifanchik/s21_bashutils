all: s21_grep

s21_grep: s21_grep.o
	gcc -o s21_grep s21_grep.o

s21_grep.o: s21_grep.c
	gcc -Wall -Werror -Wextra -std=c11 -g -c s21_grep.c -o s21_grep.o

clean:
	rm -f s21_grep.o s21_grep

rebuild: clean all