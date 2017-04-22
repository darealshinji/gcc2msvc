#!/bin/sh
set -e

tst="tmp_test"

cat <<EOF> ${tst}.cpp
#include <iostream>
int main(){ std::wcout << "SUCCESS" << std::endl; return 0; }
EOF
cat <<EOF> ${tst}.c
#include <stdio.h>
#include <wchar.h>
wchar_t *wch = L"SUCCESS";
int main(){ fprintf(stdout, "%ls\n", wch); return 0; }
EOF

for file in "${tst}.c" "${tst}.cpp" ; do
  ./gcc2msvc --verbose -Wall -g -O0 -Wcl,/EHsc $file -o ${file}.exe
done

echo ""
echo "=== Testing ${tst}.c.exe ==="
./${tst}.c.exe

echo ""
echo "=== Testing ${tst}.cpp.exe ==="
./${tst}.cpp.exe

echo ""
echo "=== Testing path conversion ==="
g++ -g -O2 -DTEST -o gcc2msvc_test run_tests.cpp gcc2msvc.cpp
./gcc2msvc_test
echo ""

