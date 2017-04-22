// compile x86 on x64

#include "config_vs2017_common.h"

#define DEFAULT_LIBPATHS  "/libpath:'.' " \
                          "/libpath:'" LIBPATH_UCRT "/x86' " \
                          "/libpath:'" LIBPATH_UM "/x86' " \
                          "/libpath:'" LIBPATH_ONECORE "'"
#define DEFAULT_CL_CMD    DEFAULT_BINPATH "/cl.exe"

