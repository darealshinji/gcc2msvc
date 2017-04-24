#include <string>

#ifdef TEST
void run_tests(void);
#endif

bool begins(const char *p, const char *str);
std::string win_path(char *ch);
std::string win_path_str(std::string str);
std::string unix_path(std::string str);
void errmsg(std::string msg);
void warnmsg(std::string msg);
void print_help(char *self);

