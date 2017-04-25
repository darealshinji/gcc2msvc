gcc2msvc
========

This program is a wrapper for msvc's cl.exe and intended to be used with Windows 10's "Bash on Ubuntu on Windows" shell.
It is invoked with gcc options (only a limited number) and turns them into msvc options to call cl.exe.
The msvc options may not exactly do the same as their gcc counterparts.


Configuration
-------------

The _default_ search paths are set at compile time like this:
``` C
#define DEFAULT_INCLUDES      "/I'.' /I'c:/Program Files/<...>/include'"
#define DEFAULT_LIBPATHS_X64  "/libpath:'c:/<...>/lib' /libpath:'c:/Program Files/<...>/lib/x64'"
#define DEFAULT_LIBPATHS_X86  "/libpath:'c:/<...>/lib' /libpath:'c:/Program Files/<...>/lib/x86'"
// path to cl.exe can also be a Windows style path
#define DEFAULT_CL_CMD_X64    "/mnt/c/Program Files/<...>/bin/x64/cl.exe'"
#define DEFAULT_CL_CMD_X86    "/mnt/c/Program Files/<...>/bin/x86/cl.exe'"
```
Support for ARM cross compiler is not explicitly implemented.
But specifying the needed paths through command line options and/or environment variables should work.


Downloads
---------

Instead of installing the full Visual Studio IDE you can also get the standalone command line
build tools at Visual Studio's homepage: http://landinghub.visualstudio.com/visual-cpp-build-tools

