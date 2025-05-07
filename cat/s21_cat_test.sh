#!/bin/bash

my_way=$(pwd)
flags="-b -e -n -s -t --number-nonblank -E --number --squeeze-blank -T"

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

# Тесты с флагами
for flag in $flags; do
    name="Test flag $flag..."
    draw_box "$name"
    for file in "$my_way/test_files/"*; do
        ./s21_cat $flag $file > result_s21_cat.txt
        cat $flag $file > result_cat.txt
        if diff -q result_s21_cat.txt result_cat.txt >/dev/null; then
            echo -e "✅ \e[36m$(basename "$file")\e[0m"
        else 
            echo -e "❌ \e[36m$(basename "$file")\e[0m"
        fi
    done
done

file2="test_files/ascii-art.txt"
file1="test_files/simple.txt"

for flag in $flags; do
    name="Test flag $flag with multiple files..."
    draw_box "$name"
    ./s21_cat $flag $file1 $file2 > result_s21_cat.txt
    cat $flag $file1 $file2 > result_cat.txt
    if diff -q result_s21_cat.txt result_cat.txt >/dev/null; then
        echo -e "✅ \e[36m$(basename "$file1") $(basename "$file2")\e[0m"
    else 
        echo -e "❌ \e[36m$(basename "$file1") $(basename "$file2")\e[0m"
    fi
done

# Тест без флагов с одиночными файлами
draw_box "Test without flags with single files..."
for file in "$my_way/test_files/"*; do
    ./s21_cat "$file" > result_s21_cat.txt
    cat "$file" > result_cat.txt
    if diff -q result_s21_cat.txt result_cat.txt >/dev/null; then
        echo -e "✅ \e[36m$(basename "$file")\e[0m"
    else
        echo -e "❌ \e[36m$(basename "$file")\e[0m"
    fi
done

# Тест без флагов с несколькими файлами
draw_box "Test without flags with multiple files..."
./s21_cat "$file1" "$file2" > result_s21_cat.txt
cat "$file1" "$file2" > result_cat.txt
if diff -q result_s21_cat.txt result_cat.txt >/dev/null; then
    echo -e "✅ \e[36m$(basename "$file1") $(basename "$file2")\e[0m"
else
    echo -e "❌ \e[36m$(basename "$file1") $(basename "$file2")\e[0m"
fi
