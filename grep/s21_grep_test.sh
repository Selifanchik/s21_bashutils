#!/bin/bash

my_way="./test_files/"
flags="-e -i -v -c -l -n -h -s -o"
combinations="-iv -nv -on -io -cv -lv -li -vo -hv -sv -hsi -cl -lvi"

template1="apple"
template2="ana"

res_grep="result_grep.txt"
s21_res_grep="result_s21_grep.txt"

draw_box() {
    text="$1"
    length=${#text}
    printf '+'
    for ((i = 0; i < length + 2; i++)); do printf '-'; done
    printf '+\n'
    printf "| %s |\n" "$text"
    printf '+'
    for ((i = 0; i < length + 2; i++)); do printf '-'; done
    printf '+\n'
}

compare_files() {
    local file1="$1"
    local file2="$2"
    if diff -q "$file1" "$file2" >/dev/null; then
        echo "[OK]"
    else
        echo "[FAIL]"
        echo "Различия:"
        diff "$file1" "$file2"
        echo "-----------------------------"
    fi
}

#Test with single file
for flag in $flags; do
    name="Test flag $flag with single files..."
    draw_box "$name"
    for file in "$my_way"*; do
        ./s21_grep $flag $template1 "$file" > "$s21_res_grep"
        grep $flag $template1 "$file" > "$res_grep"
        echo -n "$(basename "$file"): "
        compare_files "$s21_res_grep" "$res_grep"
    done
done

# Test with two files
for combo in $combinations; do
    name="Test flag $combo with two files..."
    draw_box "$name"
    for file1 in "$my_way"*; do
		for file2 in "$my_way"*; do
			if [ "$file1" != "$file2" ]; then
                ./s21_grep $combo "$template2" "$file1" "$file2" > result_s21_grep.txt
                grep $combo "$template2" "$file1" "$file2" > result_grep.txt
                echo -n "$(basename "$file1") & $(basename "$file2") : "
                compare_files result_s21_grep.txt result_grep.txt
			fi
		done
    done
done

template="pattern.txt"
#Test for -f
name="Test flag -f ..."
draw_box "$name"
for file in "$my_way"*; do
	./s21_grep -f $template "$file" > "$s21_res_grep"
	grep -f $template "$file" > "$res_grep"
	echo -n "$(basename "$file"): "
	compare_files "$s21_res_grep" "$res_grep"
done