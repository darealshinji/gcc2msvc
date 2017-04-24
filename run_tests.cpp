#include <iostream>
#include <string>

#include "gcc2msvc.hpp"

void run_tests()
{
  std::string result;

#define RUN_TEST(function, input, expect) \
  result = function(input); \
  std::cout << input << " => " << expect; \
  if (result == expect) { \
    std::cout << " [OK]" << std::endl; \
  } else { \
    std::cout << " [FAIL]" << std::endl; \
    std::cout << "result was: " << result << std::endl; \
  }

# define CH (char *)
  std::cout << "\ntesting win_path():" << std::endl;
  RUN_TEST(win_path, CH"/usr/include", "./usr/include");
  RUN_TEST(win_path, CH"/", "./");
  RUN_TEST(win_path, CH"mnt", "mnt");
  RUN_TEST(win_path, CH"/mnt", "./mnt");
  RUN_TEST(win_path, CH"/mnt/", "./mnt/");
  RUN_TEST(win_path, CH"/mnt/d", "d:/");
  RUN_TEST(win_path, CH"/mnt/d/", "d:/");
  RUN_TEST(win_path, CH"/mnt/d/dir", "d:/dir");

  std::cout << "\ntesting win_path_str():" << std::endl;
  RUN_TEST(win_path_str, "/mnt/d/dir/", "d:/dir/");
  RUN_TEST(win_path_str, "/mnt/dd", "./mnt/dd");
  RUN_TEST(win_path_str, "/mnt/dd/", "./mnt/dd/");
  RUN_TEST(win_path_str, "/mnt/D", "./mnt/D");
  RUN_TEST(win_path_str, "/mnt/D/", "./mnt/D/");
  RUN_TEST(win_path_str, "/mnt/./d", "./mnt/./d");
  RUN_TEST(win_path_str, "/mnt/./d/", "./mnt/./d/");
  RUN_TEST(win_path_str, "/mnt/../d", "./mnt/../d");
  RUN_TEST(win_path_str, "/mnt/../d/", "./mnt/../d/");

  std::cout << "\ntesting unix_path():" << std::endl;
  RUN_TEST(unix_path, CH"c:", "/mnt/c");
  RUN_TEST(unix_path, CH"c:/", "/mnt/c/");
  RUN_TEST(unix_path, CH"D:/Program Files (x86)/", "/mnt/d/Program Files (x86)/");
  RUN_TEST(unix_path, CH"d:/Program Files (x86)/", "/mnt/d/Program Files (x86)/");
}

