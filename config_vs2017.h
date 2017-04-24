#include "config_common.h"

#define DEFAULT_INCLUDES  "/I'.' " \
                          "/I'c:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.10.25017/include' " \
                          WINKITS_INCLUDES

#define LIBPATH_ONECORE   "c:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.10.25017/lib/onecore"

#define CL_CMD_PREFIX        "/mnt/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.10.25017/bin"
#define DEFAULT_CL_CMD_X64   CL_CMD_PREFIX "/HostX64/x64/cl.exe"
#define DEFAULT_CL_CMD_X86   CL_CMD_PREFIX "/HostX86/x86/cl.exe"

#define DEFAULT_LIBPATHS_X64 "/libpath:'" LIBPATH_UCRT "/x64' " \
                             "/libpath:'" LIBPATH_UM "/x64' " \
                             "/libpath:'" LIBPATH_ONECORE "/x64'"

#define DEFAULT_LIBPATHS_X86 "/libpath:'" LIBPATH_UCRT "/x86' " \
                             "/libpath:'" LIBPATH_UM "/x86' " \
                             "/libpath:'" LIBPATH_ONECORE "/x86'"

