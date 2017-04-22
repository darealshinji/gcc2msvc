#!/bin/sh
set -e

cat <<EOF> test.cpp
#include <iostream>
int main(){ std::wcout << "SUCCESS" << std::endl; return 0; }
EOF
cat <<EOF> test.c
#include <stdio.h>
#include <wchar.h>
wchar_t *wch = L"SUCCESS";
int main(){ fprintf(stdout, "%ls\n", wch); return 0; }
EOF

for file in "test.c" "test.cpp" ; do
  ./gcc2msvc --verbose -Wall -g -O0 -Wcl,/EHsc $file -o ${file}.exe
done

echo ""
echo "=== Testing test.c.exe ==="
./test.c.exe

echo ""
echo "=== Testing test.cpp.exe ==="
./test.cpp.exe

echo ""
echo "=== Testing path conversion ==="
g++ -g -O2 -DTEST -o gcc2msvc_test gcc2msvc.cpp
./gcc2msvc_test
echo ""

