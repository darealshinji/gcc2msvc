// compile x64 on x64

#include "config_vs2017_common.h"

#define DEFAULT_LIBPATHS  "/libpath:'.' " \
                          "/libpath:'" LIBPATH_UCRT "/x64' " \
                          "/libpath:'" LIBPATH_UM "/x64' " \
                          "/libpath:'" LIBPATH_ONECORE "/amd64'"
#define DEFAULT_CL_CMD    DEFAULT_BINPATH "/amd64/cl.exe"

