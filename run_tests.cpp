#include <iostream>
#include <string>

std::string win_path(char *ch);
std::string unix_path(char *ch);


void run_tests()
{
  std::string result;

#define _T(function, input, expect) \
  result = function((char *)input); \
  if (result == expect) { \
    std::cout << " [OK] "; \
  } else { \
    std::cout << " [FAIL] "; \
  } \
  std::cout << input << " => " << expect << std::endl; \
  if (result != expect) { \
    std::cout << "result was: " << result << std::endl; \
  }

  std::cout << "\ntesting win_path():" << std::endl;
  _T(win_path, "/usr/include", "./usr/include");
  _T(win_path, "/", "./");
  _T(win_path, "mnt", "mnt");
  _T(win_path, "/mnt", "./mnt");
  _T(win_path, "/mnt/", "./mnt/");
  _T(win_path, "/mnt/d", "D:/");
  _T(win_path, "/mnt/d/", "D:/");
  _T(win_path, "/mnt/d/dir", "D:/dir");
  _T(win_path, "/mnt/d/dir/", "D:/dir/");
  _T(win_path, "/mnt/dd", "./mnt/dd");
  _T(win_path, "/mnt/dd/", "./mnt/dd/");
  _T(win_path, "/mnt/D", "./mnt/D");
  _T(win_path, "/mnt/D/", "./mnt/D/");
  _T(win_path, "/mnt/./d", "./mnt/./d");
  _T(win_path, "/mnt/./d/", "./mnt/./d/");
  _T(win_path, "/mnt/../d", "./mnt/../d");
  _T(win_path, "/mnt/../d/", "./mnt/../d/");

  std::cout << "\ntesting unix_path():" << std::endl;
  _T(unix_path, "c:", "/mnt/c");
  _T(unix_path, "C:/", "/mnt/c/");
  _T(unix_path, "C:\\dir\\subdir\\another\\dir", "/mnt/c/dir/subdir/another/dir");
  _T(unix_path, "C:\\\\dir\\subdir\\another\\dir\\", "/mnt/c/dir/subdir/another/dir/");
  _T(unix_path, "D:/Program Files (x86)/", "/mnt/d/Program Files (x86)/");
  _T(unix_path, "d:/Program Files (x86)", "/mnt/d/Program Files (x86)");
}

