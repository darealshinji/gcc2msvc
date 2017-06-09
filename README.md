gcc2msvc
========

This program is a wrapper for msvc's cl.exe and intended to be used with Windows 10's "Bash on Ubuntu on Windows" shell.
It is invoked with gcc options (only a limited number) and turns them into msvc options to call cl.exe.
The msvc options may not exactly do the same as their gcc counterparts.


Configuration
-------------

The _default_ search paths are set at compile time through header files.
See the files in the `config` directory for examples.


Downloads
---------

Instead of installing the full Visual Studio IDE you can also get the standalone command line
build tools at Visual Studio's homepage: http://landinghub.visualstudio.com/visual-cpp-build-tools

