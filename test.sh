#!/bin/sh
set -e

tst="tmp_test"

cat <<EOF> ${tst}.cpp
#include <iostream>
int main(){ std::wcout << ">> SUCCESS" << std::endl; return 0; }
EOF
cat <<EOF> ${tst}.c
#include <stdio.h>
#include <wchar.h>
wchar_t *wch = L">> SUCCESS";
int main(){ fprintf(stdout, "%ls\n", wch); return 0; }
EOF

for m in 32 64 ; do
  for file in "${tst}.c" "${tst}.cpp" ; do
    ./gcc2msvc --verbose -m$m -Wall -g -O0 -Wcl,/EHsc $file -o ${file}.${m}.exe
  done
done

for m in 32 64 ; do
  echo ""
  exe="${tst}.c.${m}.exe"
  echo "=== Testing $exe ==="
  printf "file type: "
  file -b $exe
  ./$exe

  echo ""
  exe="${tst}.cpp.${m}.exe"
  echo "=== Testing $exe ==="
  printf "file type: "
  file -b $exe
  ./$exe
done

echo ""
echo "=== Testing path conversion ==="
g++ -g -O2 -DTEST -o gcc2msvc_test run_tests.cpp main.cpp
./gcc2msvc_test
echo ""

