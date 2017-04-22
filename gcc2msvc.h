#include <string>

#ifdef TEST
void test_win_path();
#endif

bool begins(const char *p, const char *str);
std::string win_path(char *ch);
std::string unix_path(std::string str);
void errmsg(std::string msg);
void print_help(char *self);

