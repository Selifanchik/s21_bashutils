#!/bin/bash

# Путь к тестовым файлам
my_way=$(pwd)
flags="-e -i -v -c -l -n -h -s -o"
combinations=(
    "-i -v"
    "-n -v"
    "-o -n"
    "-i -o"
    "-c -v"
    "-l -v"
    "-l -i"
	"-v -o"
	"-h -v"
	"-s -h"
    "-c -l"
)

# Функция для красивого отображения текста в рамке
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

# Шаблон для поиска
template="apple"

# Тестирование с каждым флагом и несколькими файлами
for flag in $flags; do
    name="Test flag $flag with multiple files..."
    draw_box "$name"
    for file1 in "$my_way/test_files/"*; do
        for file2 in "$my_way/test_files/"*; do
            if [ "$file1" != "$file2" ]; then
                ./s21_grep $flag $template $file1 $file2 > result_s21_grep.txt
                grep $flag $template $file1 $file2 -E > result_grep.txt
                if diff -q result_s21_grep.txt result_grep.txt >/dev/null; then
                    echo -e "✅ \e[36m$(basename "$file1") and $(basename "$file2")\e[0m"
                else
                    echo -e "❌ \e[36m$(basename "$file1") and $(basename "$file2")\e[0m"
                fi
            fi
        done
    done
done

# Тестирование с комбинациями флагов (включая -l -v и -l -i)
for combo in "${combinations[@]}"; do
    name="Test flags $combo..."
    draw_box "$name"
    ./s21_grep $combo $template "$my_way/test_files/"* > result_s21_grep.txt
    grep $combo $template "$my_way/test_files/"* -E > result_grep.txt
    if diff -q result_s21_grep.txt result_grep.txt >/dev/null; then
        echo -e "✅ \e[36m$combo\e[0m"
    else
        echo -e "❌ \e[36m$combo\e[0m"
    fi
done

# Тестирование с флагом -f (чтение шаблона из файла)
draw_box "Test flag -f (template from file)..."
echo "$template" > pattern.txt  # Создаем файл с шаблоном
for file in "$my_way/test_files/"*; do
    ./s21_grep -f pattern.txt $file > result_s21_grep.txt
    grep -f pattern.txt $file -E > result_grep.txt
    if diff -q result_s21_grep.txt result_grep.txt >/dev/null; then
        echo -e "✅ \e[36m$(basename "$file")\e[0m"
    else
        echo -e "❌ \e[36m$(basename "$file")\e[0m"
    fi
done
rm pattern.txt  # Удаляем файл с шаблоном после теста

# Тестирование с флагами -o (вывод только совпадений) и -n (вывод номеров строк)
draw_box "Test flags -o and -n..."
for file in "$my_way/test_files/"*; do
    ./s21_grep -o -n $template $file > result_s21_grep.txt
    grep -o -n $template $file -E > result_grep.txt
    if diff -q result_s21_grep.txt result_grep.txt >/dev/null; then
        echo -e "✅ \e[36m$(basename "$file")\e[0m"
    else
        echo -e "❌ \e[36m$(basename "$file")\e[0m"
    fi
done

# Тестирование с флагом -s (не выводить сообщения об ошибках)
draw_box "Test flag -s (suppress errors)..."
for file in "$my_way/test_files/"*; do
    ./s21_grep -s $template $file > result_s21_grep.txt
    grep -s $template $file -E > result_grep.txt
    if diff -q result_s21_grep.txt result_grep.txt >/dev/null; then
        echo -e "✅ \e[36m$(basename "$file")\e[0m"
    else
        echo -e "❌ \e[36m$(basename "$file")\e[0m"
    fi
done
