#!/bin/bash

my_way=$(pwd)
flags="-e -i -v -c -l -n"

draw_box() {
    text="$1"
    length=${#text}
    printf '+'
    for ((i = 0; i < length + 2; i++)); do printf '-'; done
    printf '+\n'
    printf "| \e[33m%s\e[0m |\n" "$text"
    printf '+'
    for ((i = 0; i < length + 2; i++)); do printf '-'; done
    printf '+\n'
}

template="a+b"

for flag in $flags; do
	name="Test flag $flag..."
	draw_box "$name"
	for file in "$my_way/test_files/"*; do
		./s21_grep $flag $template $file > result_s21_grep.txt
		grep $flag $template $file -E > result_grep.txt
		if diff -q result_s21_grep.txt result_grep.txt >/dev/null; then
			echo -e "✅ \e[36m$(basename "$file")\e[0m"
		else 
			echo -e "❌ \e[36m$(basename "$file")\e[0m"
		fi
	done
done

name="Test without flag ..."
draw_box "$name"
for file in "$my_way/test_files/"*; do
	./s21_grep $template $file > result_s21_grep.txt
	grep $template $file -E > result_grep.txt
	if diff -q result_s21_grep.txt result_grep.txt >/dev/null; then
		echo -e "✅ \e[36m$(basename "$file")\e[0m"
	else 
		echo -e "❌ \e[36m$(basename "$file")\e[0m"
	fi
done