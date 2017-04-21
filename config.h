#ifndef MSVC_VER
# define MSVC_VER "14.10.25017"
#endif
#ifndef WINKIT_VER
# define WINKIT_VER "10.0.15063.0"
#endif

#ifndef MSVC_HOST
# define MSVC_HOST "HostX64"
#endif
#ifndef MSVC_TARGET
# define MSVC_TARGET "x64"
#endif
#ifndef PROGRAM_FILES
# define PROGRAM_FILES "Program Files (x86)"
#endif

#ifndef DEFAULT_INCLUDES
# define DEFAULT_INCLUDES "/I'.' " \
    "/I'c:\\" PROGRAM_FILES "\\Microsoft Visual Studio\\2017\\Community\\VC\\Tools\\MSVC\\" MSVC_VER "\\include' " \
    "/I'c:\\" PROGRAM_FILES "\\Windows Kits\\10\\Include\\" WINKIT_VER "\\shared' " \
    "/I'c:\\" PROGRAM_FILES "\\Windows Kits\\10\\Include\\" WINKIT_VER "\\ucrt' " \
    "/I'c:\\" PROGRAM_FILES "\\Windows Kits\\10\\Include\\" WINKIT_VER "\\um'"
#endif

#ifndef DEFAULT_LIBPATHS
# define DEFAULT_LIBPATHS "/libpath:'.' " \
    "/libpath:'c:\\" PROGRAM_FILES "\\Windows Kits\\10\\Lib\\" WINKIT_VER "\\ucrt\\" MSVC_TARGET "' " \
    "/libpath:'c:\\" PROGRAM_FILES "\\Windows Kits\\10\\Lib\\" WINKIT_VER "\\um\\" MSVC_TARGET "' " \
    "/libpath:'c:\\" PROGRAM_FILES "\\Microsoft Visual Studio\\2017\\Community\\VC\\Tools\\MSVC\\" MSVC_VER "\\lib\\" MSVC_TARGET "'"
#endif

#ifndef DEFAULT_CL_CMD
//# define DEFAULT_CL_CMD "c:\\" PROGRAM_FILES "\\Microsoft Visual Studio\\2017\\Community\\VC\\Tools\\MSVC\\" MSVC_VER "\\bin\\" MSVC_HOST "\\" MSVC_TARGET "\\cl.exe"
# define DEFAULT_CL_CMD "/mnt/c/" PROGRAM_FILES "/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/" MSVC_VER "/bin/" MSVC_HOST "/" MSVC_TARGET "/cl.exe"
#endif

