all: s21_cat

s21_cat: s21_cat.o
	gcc -o s21_cat s21_cat.o

s21_cat.o: s21_cat.c
	gcc -Wall -Werror -Wextra -std=c11 -g -c s21_cat.c -o s21_cat.o

clean:
	rm -f s21_cat.o s21_cat

rebuild:
	clean all