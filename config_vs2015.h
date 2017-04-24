#include "config_common.h"

#define DEFAULT_INCLUDES  "/I'.' " \
                          "/I'c:/Program Files (x86)/Microsoft Visual Studio/Shared/14.0/VC/include' " \
                          WINKITS_INCLUDES

#define LIBPATH_ONECORE   "c:/Program Files (x86)/Microsoft Visual Studio/Shared/14.0/VC/lib/onecore"

#define CL_CMD_PREFIX        "/mnt/c/Program Files (x86)/Microsoft Visual Studio/Shared/14.0/VC/bin"
#define DEFAULT_CL_CMD_X64   CL_CMD_PREFIX "/amd64/cl.exe"
#define DEFAULT_CL_CMD_X86   CL_CMD_PREFIX "/cl.exe"

#define DEFAULT_LIBPATHS_X64 "/libpath:'" LIBPATH_UCRT "/x64' " \
                             "/libpath:'" LIBPATH_UM "/x64' " \
                             "/libpath:'" LIBPATH_ONECORE "/amd64'"

#define DEFAULT_LIBPATHS_X86 "/libpath:'" LIBPATH_UCRT "/x86' " \
                             "/libpath:'" LIBPATH_UM "/x86' " \
                             "/libpath:'" LIBPATH_ONECORE "'"

