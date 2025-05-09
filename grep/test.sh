#!/bin/bash

MY_GREP=./s21_grep

mkdir -p test_files

echo -e "apple\norange\ngrape\napple pie\nbanana\nApple" > test_files/file1.txt
echo -e "cat\ndog\nmouse\nfish\napple\nbanana" > test_files/file2.txt

TESTS=(
    "-i -n -o apple test_files/file1.txt test_files/file2.txt"
    "-v -c -h banana test_files/file1.txt test_files/file2.txt"
    "-o -l -i cat test_files/file1.txt test_files/file2.txt"
    "-i -n -o -s -e apple test_files/file1.txt test_files/file2.txt"
    "-v -c -l -h -e grape test_files/file1.txt test_files/file2.txt"
    "-i -n -o -l -e fish test_files/file1.txt test_files/file2.txt"
)

i=1

for test_case in "${TESTS[@]}"; do
    echo "Тест $i: $test_case"
    grep $test_case > grep_result.txt 2> grep_error.txt
    $MY_GREP $test_case > my_result.txt 2> my_error.txt

    diff -u grep_result.txt my_result.txt > result_diff.txt
    diff -u grep_error.txt my_error.txt > error_diff.txt

    if [[ -s result_diff.txt || -s error_diff.txt ]]; then
        echo "❌ Отличия найдены в тесте $i"
        cat result_diff.txt
        cat error_diff.txt
    else
        echo "✅ Совпадает"
    fi

    echo "-----------------------------"
    ((i++))
done

rm -f grep_result.txt my_result.txt grep_error.txt my_error.txt result_diff.txt
