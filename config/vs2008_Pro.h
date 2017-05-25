/* Visual Studio 2008 (Pro) */

#define DEFAULT_INCLUDES \
 "/I'.' " \
 "/I'C:/Program Files (x86)/Microsoft Visual Studio 9.0/VC/include' " \
 "/I'C:/Program Files/Microsoft SDKs/Windows/v6.0A/Include'"

#define DEFAULT_CL_CMD_X64 "/mnt/c/Program Files (x86)/Microsoft Visual Studio 9.0/VC/bin/amd64/cl.exe"

/* You need to copy the following files from "C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE"
 * to "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin" to ensure the x86 cl.exe
 * binary can be run from Bash:
 * msdis160.dll msobj80.dll mspdb80.dll mspdbcore.dll mspdbsrv.exe */
#define DEFAULT_CL_CMD_X86 "/mnt/c/Program Files (x86)/Microsoft Visual Studio 9.0/VC/bin/cl.exe"

#define DEFAULT_LIBPATHS_X64 \
 "/libpath:'C:/Program Files (x86)/Microsoft Visual Studio 9.0/VC/lib/amd64' " \
 "/libpath:'C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/x64'"

#define DEFAULT_LIBPATHS_X86 \
 "/libpath:'C:/Program Files (x86)/Microsoft Visual Studio 9.0/VC/lib' " \
 "/libpath:'C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib'"
