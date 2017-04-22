#include <iostream>
#include <string>

#include "gcc2msvc.hpp"


void run_tests()
{
# define CH (char *)
  int test_dirs_nix_to_win = 17;
  int test_dirs_win_to_nix = 4;
  int test_length = test_dirs_nix_to_win + test_dirs_win_to_nix;

  char *test_dirs[test_length+1] = {
    CH"/usr/include",
    CH"/",
    CH"mnt",
    CH"/mnt",
    CH"/mnt/",
    CH"/mnt/d",
    CH"/mnt/d/",
    CH"/mnt/d/dir",
    CH"/mnt/d/dir/",
    CH"/mnt/dd",
    CH"/mnt/dd/",
    CH"/mnt/D",
    CH"/mnt/D/",
    CH"/mnt/./d",
    CH"/mnt/./d/",
    CH"/mnt/../d",
    CH"/mnt/../d/",
    CH"c:",
    CH"c:/",
    CH"C:/Program Files (x86)/",
    CH"c:/Program Files (x86)/",
    NULL
  };

  std::string test_expect[test_length] = {
    "./usr/include",
    "./",
    "mnt",
    "./mnt",
    "./mnt/",
    "d:/",
    "d:/",
    "d:/dir",
    "d:/dir/",
    "./mnt/dd",
    "./mnt/dd/",
    "./mnt/D",
    "./mnt/D/",
    "./mnt/./d",
    "./mnt/./d/",
    "./mnt/../d",
    "./mnt/../d/",
    "/mnt/c",
    "/mnt/c/",
    "/mnt/c/Program Files (x86)/",
    "/mnt/c/Program Files (x86)/",
  };

#define PRINT_RESULTS(func) \
  std::string result = func(test_dirs[i]); \
  std::cout << test_dirs[i] << " => " << test_expect[i]; \
  if (result == test_expect[i]) { \
    std::cout << " [OK]" << std::endl; \
  } else { \
    std::cout << " [FAIL]" << std::endl; \
    std::cout << "result was: " << result << std::endl; \
  }

  for (int i = 0; i < test_dirs_nix_to_win; ++i)
  {
    PRINT_RESULTS(win_path);
  }
  for (int i = test_dirs_nix_to_win; i < test_length; ++i)
  {
    PRINT_RESULTS(unix_path);
  }
}

