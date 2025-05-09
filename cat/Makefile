all: format s21_cat

s21_cat: s21_cat.o
	gcc -o s21_cat s21_cat.o

s21_cat.o: s21_cat.c
	gcc -Wall -Werror -Wextra -std=c11 -g -c s21_cat.c -o s21_cat.o

format:
	clang-format -i -style=Google *.c
	clang-format -i -style=Google *.h

clean:
	rm -f s21_cat.o s21_cat

rebuild: clean all