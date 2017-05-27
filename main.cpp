/**
 * The MIT License (MIT)
 *
 * Copyright (C) 2017, djcj <djcj@gmx.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define USAGE "\n" \
  "This program is a wrapper for msvc's cl.exe and intended to be used\n" \
  "with Windows 10's \"Bash on Ubuntu on Windows\" shell.\n" \
  "It is invoked with gcc options (only a limited number) and turns\n" \
  "them into msvc options to call cl.exe.\n" \
  "The msvc options may not exactly do the same as their gcc counterparts.\n" \
  "\n" \
  "Supported GCC options (see `man gcc' for more information):\n" \
  "  -c -C -DDEFINE[=ARG] -fconstexpr-depth=num -ffp-contract=fast|off\n" \
  "  -finline-functions -fno-inline -frtti -fthreadsafe-statics\n" \
  "  -fomit-frame-pointer -fopenmp -fpermissive -fsized-deallocation -fstack-check\n" \
  "  -fstack-protector -funsigned-char -fwhole-program -g -include file -I path\n" \
  "  -llibname -L path -m32 -mavx -mavx2 -mdll -msse -msse2 -nodefaultlibs -nostdinc\n" \
  "  -nostdinc++ -nostdlib -O0 -O1 -O2 -O3 -Os -o file -print-search-dirs -shared\n" \
  "  -std=c<..>|gnu<..> -trigraphs -UDEFINE -w -Wall -Werror -Wextra\n" \
  "  -Wl,--out-implib,libname -Wl,-output-def,defname -Wl,--whole-archive -x <c|c++>\n" \
  "\n" \
  "Other options:\n" \
  "  --help                display this information\n" \
  "  --help-cl             display cl.exe's help information\n" \
  "  --help-link           display link.exe's help information\n" \
  "  --version             display version information of cl.exe and link.exe\n" \
  "  --verbose             print commands\n" \
  "  --print-only          print commands and don't to anything\n" \
  "  --cl=path             path to cl.exe\n" \
  "  -Wcl,arg -Wlink,arg   parse msvc options directly to cl.exe/link.exe;\n" \
  "                        see also https://msdn.microsoft.com/en-us/library/19z1t1wy.aspx\n" \
  "\n" \
  "Environment variables:\n" \
  "  CL_CMD      path to cl.exe\n" \
  "  INCLUDE     semicolon (;) separated list of include paths\n" \
  "  LIB         semicolon (;) separated list of library search paths\n"

#include <iostream>
#include <string>

#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "config.h"

#define STR(x) std::string(x)

#ifdef TEST
void run_tests(void);
#endif
bool begins(const char *p, const char *str);
std::string win_path(char *ch);
std::string unix_path(char *ch);
void replace_with_forward_slashes(const std::string &from, std::string &str);
void split_env(const char *env_var, std::string msvc_arg, std::string &str);
void print_help(char *self);
extern "C" {
int system_return(const char *command);
}


/* check if the beginning of p equals str and if p is longer than str */
bool begins(const char *p, const char *str)
{
  size_t n = strlen(str);

  if (strncmp(p, str, n) == 0 && strlen(p) > n)
  {
    return true;
  }
  return false;
}

/* C: is mounted as "/mnt/c", D: as "/mnt/d", and so on;
 * forward slashes (/) are not converted to backslashes (\)
 * because Windows actually supports them */

std::string win_path(char *ch)
{
  std::string str;

  if (ch[0] == '/')
  {
    bool prepend = true;

    if (begins(ch, "/mnt/"))
    {
      if (strchr("cdefghijklmnopqrstuvwxyzab", ch[5]) != NULL)
      {
        char *drive = new char[2];
        sprintf(drive, "%c", toupper(ch[5]));

        if (strlen(ch) == 6)
        {
          /* /mnt/d -> D:/ */
          prepend = false;
          str = STR(drive) + ":/";
        }
        else if (ch[6] == '/')
        {
          /* /mnt/d/ -> D:/
           * /mnt/d/dir -> D:/dir */
          prepend = false;
          str = STR(drive) + ":/" + STR(ch+7);
        }
      }
    }

    if (prepend)
    {
      /* /usr/include -> ./usr/include */
      str = "." + STR(ch);
    }
  }
  else
  {
    str = STR(ch);
  }

  return str;
}

std::string unix_path(char *ch)
{
  std::string str;
  char drive = '\0';
  size_t len = strlen(ch);

  /* check for drive letter */
  if (len >= 2 && ch[1] == ':')
  {
    if (len == 2 || (len > 2 && (ch[2] == '\\' || ch[2] == '/')))
    {
      if (strchr("CDEFGHIJKLMNOPQRSTUVWXYZAB", ch[0]) != NULL)
      {
        drive = tolower(ch[0]);
      }
      else if (strchr("cdefghijklmnopqrstuvwxyzab", ch[0]) != NULL)
      {
        drive = ch[0];
      }
    }
  }

  if (drive != '\0')
  {
    char *tmp = new char[2];
    sprintf(tmp, "%c", drive);

    if (len == 2)
    {
      str = "/mnt/" + STR(tmp);
    }
    else
    {
      /* don't use "/mnt/%c/" to prevent double slashes */
      str = "/mnt/" + STR(tmp) + "\\" + STR(ch+3);
    }
  }
  else
  {
    str = STR(ch);
  }

  /* convert backslashes (\) to forward slashes (/) */
  replace_with_forward_slashes("\\\\", str);
  replace_with_forward_slashes("\\", str);

  return str;
}

void replace_with_forward_slashes(const std::string &from, std::string &str)
{
  for (size_t i = 0; (i = str.find(from, i)) != std::string::npos; ++i)
  {
    str.replace(i, from.size(), "/");
  }
}

void split_env(const char *env_var, std::string msvc_arg, std::string &str)
{
  char *env = getenv(env_var);
  if (env != NULL)
  {
    char *token = strtok(env, ";");
    while (token != NULL)
    {
      str += " /" + msvc_arg + "'" + win_path(token) + "'";
      token = strtok(NULL, ";");
    }
  }
}

void print_help(char *self)
{
  std::cout << "Usage: " << self << " [options] file...\n" << USAGE << std::endl;
}


int main(int argc, char **argv)
{
#ifdef TEST
  run_tests();
  return 0;
#endif

  std::string str, cmd, lnk, driver;
  std::string driver_default = DEFAULT_CL_CMD_X64;
  std::string includes_default = DEFAULT_INCLUDES;
  std::string lib_paths_default = DEFAULT_LIBPATHS_X64;
  int bits = 64;

  bool verbose = false;
  bool print_only = false;
  bool have_outname = false;
  bool print_search_dirs = false;
  bool print_help_cl = false;
  bool print_help_link = false;
  bool print_version = false;
  bool do_link = true;
  bool use_default_driver = true;
  bool use_default_inc_paths = true;
  bool default_lib_paths = true;
  bool dll = false;

  char *driver_env = getenv("CL_CMD");
  if (driver_env != NULL)
  {
    driver = unix_path(driver_env);
  }


  /* parse arguments */

  for (int i = 1; i < argc; ++i)
  {
    size_t len = strlen(argv[i]);
    char *arg = argv[i];
    str = STR(argv[i]);

    if (arg[0] == '-')
    {
      if (arg[1] == '-')
      {
        if      (begins(arg, "--cl="))   { driver = unix_path(arg+5);
                                           use_default_driver = false;    }
        else if (str == "--verbose")     { verbose = true;                }
        else if (str == "--print-only")  { verbose = print_only = true;   }
        else if (str == "--help")        { print_help(argv[0]); return 0; }
        else if (begins(arg, "--help-"))
        {
          if      (str == "--help-cl")   { print_help_cl = true;   }
          else if (str == "--help-link") { print_help_link = true; }
        }
        else if (str == "--version")     { print_version = true;   }
      }
      else
      {
        if ((arg[1] == '?' || arg[1] == 'h') && (len == 2 || str == "-help"))
        {
          print_help(argv[0]);
          return 0;
        }

        /*  -c -C -w  */
        else if (str == "-c" || str == "-C" || str == "-w")
        {
          cmd += " /" + std::string(arg+1);
          if (str == "-c") {
            do_link = false;
          }
        }

        /*  -g  */
        else if (str == "-g")
        {
          cmd += " /Zi";
        }

        /*  -x c  -x c++  */
        else if (arg[1] == 'x')
        {
          if (len == 2) {
            ++i;
            if (i < argc) {
              if      (STR(argv[i]) == "c")   { cmd += " /TC"; }
              else if (STR(argv[i]) == "c++") { cmd += " /TP"; }
            }
          }
          else if (str == "-xc")   { cmd += " /TC"; }
          else if (str == "-xc++") { cmd += " /TP"; }
        }

        /*  -o file  */
        else if (arg[1] == 'o')
        {
          if (len == 2) {
            ++i;
            if (i < argc) {
              lnk += " /out:'" + STR(argv[i]) + "'";
            }
          } else {
            lnk += " /out:'" + STR(arg+2) + "'";
          }
          have_outname = true;
        }

        /*  -I path  */
        else if (arg[1] == 'I')
        {
          if (len == 2) {
            ++i;
            if (i < argc) {
              cmd += " /I'" + win_path(argv[i]) + "'";
            }
          } else {
            cmd += " /I'" + win_path(arg+2) + "'";
          }
        }

        /*  -DDEFINE[=ARG]  -UDEFINE  */
        else if (arg[1] == 'D' || arg[1] == 'U')
        {
          if (len == 2) {
            ++i;
            if (i < argc) {
              cmd += " /" + str.substr(1,1) + "'" + STR(argv[i]) + "'";
            }
          } else {
            cmd += " /" + str.substr(1,1) + "'" + STR(arg+2) + "'";
          }
        }

        /*  -L path  */
        else if (arg[1] == 'L')
        {
          if (len == 2) {
            ++i;
            if (i < argc) {
              lnk += " /libpath:'" + win_path(argv[i]) + "'";
            }
          } else {
            lnk += " /libpath:'" + win_path(arg+2) + "'";
          }
        }

        /*  -llibname  */
        else if (arg[1] == 'l' && len > 2)
        {
          if      (str == "-lmsvcrt")      { cmd += " /MD"; }
          else if (str == "-lcmt" ||
                   str == "-llibcmt")      { cmd += " /MT"; } /* however libcmt is not part of mingw-w64 */
          else if (str != "-lc"         &&
                   str != "-lm"         &&
                   str != "-lrt"        && /* always ignore these libraries */
                   str != "-lstdc++"    &&
                   str != "-lgcc_s"     &&
                   str != "-lmingw32"   && /* TODO: maybe add an option   */
                   str != "-lmingwex"   && /* to disable the blacklisting */
                   str != "-lmingwthrd" &&
                   str != "-lmoldname"  &&
                   str != "-lpthread"   &&
                   str != "-lwinpthread")  { lnk += " '" + STR(arg+2) + ".lib'"; }
        }

        /*  -O0 -O1 -O2 -O3 -Os  */
        else if (arg[1] == 'O' && len == 3)
        {
          if      (arg[2] == '1' ||
                   arg[2] == '2')   { cmd += " /O2 /Ot"; }
          else if (arg[2] == '3')   { cmd += " /Ox";     }
          else if (arg[2] == 's')   { cmd += " /O1 /Os"; }
          else if (arg[2] == '0')   { cmd += " /Od";     }
        }

        /*  -Wl,--whole-archive
         *  -Wl,--out-implib,libname
         *  -Wl,-output-def,defname
         *  -Wall  -Wextra  -Werror
         *  -Wcl,arg  -Wlink,arg  */
        else if (arg[1] == 'W' && len > 2)
        {
          if (arg[2] == 'l')
          {
            std::string lopt;
            if (len == 3) {
              ++i;
              if (i < argc) {
                lopt = STR(argv[i]);
              }
            } else {
              lopt = STR(arg+4);
            }

            if (lopt == "--whole-archive")
            {
              lnk += " /wholearchive";
            }

            else if (begins(lopt.c_str(), "--out-implib,"))
            {
              lnk += " /implib:'" + STR(arg+17) + "'";
            }
            else if (str == "-Wl,--out-implib")
            {
              ++i;
              if (i < argc && begins(argv[i], "-Wl,")) {
                lnk += " /implib:'" + STR(argv[i]+4) + "'";
              }
            }

            else if (begins(lopt.c_str(), "-output-def,"))
            {
              lnk += " /def:'" + STR(arg+16) + "'";
            }
            else if (str == "-Wl,-output-def")
            {
              ++i;
              if (i < argc && begins(argv[i], "-Wl,")) {
                lnk += " /def:'" + STR(argv[i]+4) + "'";
              }
            }

            else if (str == "-Wlink" || begins(arg, "-Wlink,"))
            {
              std::string s;
              if (str == "-Wlink" && i+1 < argc)
              {
                ++i;
                s = STR(argv[i]);
              }
              else
              {
                s = STR(arg+7);
              }

              if (!s.empty())
              {
                if (s[0] == '-')
                {
                  lnk += " /" + s.substr(1);
                }
                else if (s[0] != '/')
                {
                  lnk += " /" + s;
                }
                else
                {
                  lnk += " " + s;
                }
              }
            }
          }

          else if (str == "-Wcl" || begins(arg, "-Wcl,"))
          {
            std::string s;
            if (str == "-Wcl" && i+1 < argc)
            {
              ++i;
              s = STR(argv[i]);
            }
            else
            {
              s = STR(arg+5);
            }

            if (!s.empty())
            {
              if (s[0] == '-')
              {
                cmd += " /" + s.substr(1);
              }
              else if (s[0] != '/')
              {
                cmd += " /" + s;
              }
              else
              {
                cmd += " " + s;
              }
            }
          }

          else if (str == "-Wall")   { cmd += " /W3";   }
          else if (str == "-Wextra") { cmd += " /Wall"; }
          else if (str == "-Werror") { cmd += " /WX";   }
        }

        /*  -mdll  -msse -msse2  -mavx -mavx2  */
        else if (arg[1] == 'm' && len > 2)
        {
          if      (str == "-m32")   { bits = 32;                 }
          else if (str == "-m64")   { bits = 64;                 }
          else if (str == "-mdll")  { cmd += " /LD"; dll = true; }
          else if (str == "-msse")  { cmd += " /arch:SSE";       }
          else if (str == "-msse2") { cmd += " /arch:SSE2";      }
          else if (str == "-mavx")  { cmd += " /arch:AVX";       }
          else if (str == "-mavx2") { cmd += " /arch:AVX2";      }
        }

        /*  -frtti -fthreadsafe-statics -fno-inline -fomit-frame-pointer
         *  -fpermissive -finline-functions -fopenmp -fstack-protector -fstack-check
         *  -fstack-protector-strong -fstack-protector-all
         *  -funsigned-char -fsized-deallocation -fconstexpr-depth=num
         *  -ffp-contract=fast|off -fwhole-program  */
        else if (arg[1] == 'f' && len > 2)
        {
          if (begins(arg, "-fno-"))
          {
            if      (str == "-fno-rtti")                { cmd += " /GR-";                }
            else if (str == "-fno-threadsafe-statics")  { cmd += " /Zc:threadSafeInit-"; }
            else if (str == "-fno-inline")              { cmd += " /Ob0";                }
            else if (str == "-fno-stack-protector" ||
                     str == "-fno-stack-check")         { cmd += " /GS- /guard:cf-";     }
            else if (str == "-fno-sized-deallocation")  { cmd += " /Zc:sizedDealloc-";   }
            else if (str == "-fno-whole-program")       { cmd += " /GL-";                }
          }
          else
          {
            if      (str == "-fomit-frame-pointer")     { cmd += " /Oy";                 }
            else if (str == "-fpermissive")             { cmd += " /permissive";         }
            else if (str == "-fstack-protector" ||
                     str == "-fstack-check")            { cmd += " /GS";                 }
            else if (str == "-fstack-protector-strong" ||
                     str == "-fstack-protector-all")    { cmd += " /GS /guard:cf";       }
            else if (str == "-finline-functions")       { cmd += " /Ob2";                }
            else if (str == "-frtti")                   { cmd += " /GR";                 }
            else if (str == "-fthreadsafe-statics")     { cmd += " /Zc:threadSafeInit";  }
            else if (str == "-fopenmp")                 { cmd += " /openmp";             }
            else if (str == "-funsigned-char")          { cmd += " /J";                  }
            else if (str == "-fsized-deallocation")     { cmd += " /Zc:sizedDealloc";    }
            else if (begins(arg, "-fconstexpr-depth=")) { cmd += " /constexpr:depth" + STR(arg+18); }
            else if (begins(arg, "-ffp-contract="))
            {
              if      (STR(arg+14) == "fast")           { cmd += " /fp:fast";            }
              else if (STR(arg+14) == "off")            { cmd += " /fp:strict";          }
            }
            else if (str == "-fwhole-program")          { cmd += " /GL";                 }
          }
        }

        /*  -nostdinc  -nostdinc++  -nostdlib  -nodefaultlibs  */
        else if (arg[1] == 'n' && len > 8)
        {
          if      (str == "-nostdinc" ||
                   str == "-nostdinc++")    { use_default_inc_paths = false; }
          else if (str == "-nostdlib")      { default_lib_paths = false;     }
          else if (str == "-nodefaultlibs") { lnk += " /nodefaultlib";
                                              default_lib_paths = false;     }
        }

        /*  -shared  -std=c<..>|gnu<..>  */
        else if (arg[1] == 's' && len > 5)
        {
          if      (str == "-shared")       { cmd += " /LD"; dll = true;     }
          else if (begins(arg, "-std="))
          {
            if   (begins(arg, "-std=gnu")) { cmd += " /std:c" + STR(arg+8); }
            else                           { cmd += " /std:" + STR(arg+5);  }
          }
        }

        /*  -include file  */
        else if (str == "-include")
        {
          ++i;
          if (i < argc) {
            cmd += " /FI'" + STR(argv[i]) + "'";
          }
        }

        /*  -trigraphs  */
        else if (str == "-trigraphs")
        {
          cmd += " /Zc:trigraphs";
        }

        /*  -print-search-dirs  */
        else if (str == "-print-search-dirs")
        {
          print_search_dirs = true;
        }
      }
    }
    else
    {
      cmd += " '" + win_path(arg) + "'";
    }
  }

  if (bits == 32)
  {
    if (!use_default_driver)
    {
      std::cerr << "warning: ignoring `-m32' when using a custom cl.exe" << std::endl;
    }
    driver_default = DEFAULT_CL_CMD_X86;
    lib_paths_default = DEFAULT_LIBPATHS_X86;
  }
  if (use_default_driver)
  {
    driver = driver_default;
  }


  /* print information and exit */

  if (print_help_cl)
  {
    /* piping to cat helps to display the
     * output correctly and in one go */
    cmd = "'" + driver + "' /help 2>&1 | cat";
    return system(cmd.c_str());
  }
  else if (print_help_link)
  {
    cmd = "'" + driver.substr(0, driver.find_last_of("/\\")) + "/link.exe' 2>&1 | cat";
    return system(cmd.c_str());
  }
  else if (print_version)
  {
    if (use_default_driver)
    {
      cmd = "'" DEFAULT_CL_CMD_X64 "' 2>&1 | head -n3 ; "
            "'" DEFAULT_CL_CMD_X86 "' 2>&1 | head -n3 ; ";
    }
    else
    {
      cmd = "'" + driver + "' 2>&1 | head -n3 ; ";
    }
    cmd += "'" + driver.substr(0, driver.find_last_of("/\\")) + "/link.exe' 2>&1 | head -n3";
    return system(cmd.c_str());
  }

  if (print_search_dirs)
  {
    std::cout << "cl.exe: " << driver_default << std::endl;
    std::cout << "includes: " << includes_default << std::endl;
    std::cout << "libraries: " << lib_paths_default << std::endl;
    return 0;
  }


  /* turn lists obtained from environment variables INCLUDE and
   * and LIB into command line arguments /I'dir' and /libpath'dir' */
  split_env("INCLUDE", "I", cmd);
  split_env("LIB", "libpath", cmd);


  /* create the final command to execute */

  if (use_default_inc_paths) { cmd += " " + includes_default;  }
  if (do_link)
  {
    if (!have_outname)
    {
      if (dll) { lnk += " /out:'a.dll'"; }
      else     { lnk += " /out:'a.exe'"; }
    }
    if (default_lib_paths) { lnk += " " + lib_paths_default; }
    cmd += " /link" + lnk;
  }

  cmd = "'" + driver + "'" + cmd;

  if (verbose)
  {
    std::cout << cmd << std::endl;
  }
  if (print_only)
  {
    return 0;
  }

  return system_return(cmd.c_str());
}

