#!/bin/bash

my_way="./test_files/"
flags="-b -e -n -s -t --number-nonblank -E --number --squeeze-blank -T"

file1="test_files/file4.txt"
file2="test_files/file2.txt"


res_cat="result_cat.txt"
s21_res_cat="result_s21_cat.txt"

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


#Test without flags with single files
draw_box "Test without flags with single files..."
for file in "$my_way"*; do
    ./s21_cat "$file" > "$s21_res_cat"
    cat "$file" > "$res_cat"
    echo -n "$(basename "$file"): "
    compare_files "$s21_res_cat" "$res_cat"
done

#Test with single file
for flag in $flags; do
    name="Test flag $flag..."
    draw_box "$name"
    for file in "$my_way"*; do
        ./s21_cat $flag $file > "$s21_res_cat"
        cat $flag $file > "$res_cat"
        echo -n "$(basename "$file"): "
        compare_files "$s21_res_cat" "$res_cat"
    done
done

# Test with multiple files
for flag in $flags; do
    name="Test flag $flag with multiple files..."
    draw_box "$name"
    ./s21_cat $flag $file1 $file2 > "$s21_res_cat"
    cat $flag $file1 $file2 > "$res_cat"
    echo -n "$(basename "$file1") $(basename "$file2"): "
    compare_files "$s21_res_cat" "$res_cat"
done

