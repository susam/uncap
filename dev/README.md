Uncap Developer Notes
=====================
The binary releases at <https://github.com/susam/uncap/releases> are
compiled using Microsoft Visual Studio 2005. However it is possible to
build this project using MinGW too.

Here are the version details of both development environments that have
been found to build this project successfully.

  1. gcc 4.8.1 available with MinGW 0.6.2 on Windows 7.
  2. Microsoft (R) 32-bit C/C++ Optimizing Compiler Version
     14.00.50727.762 for 80x86 (cl.exe) available with
     Microsoft Visual Studio 2005 Version 8.0.50727.4039 on Windows 7.

Build scripts are provided in this directory for both development
environments.


Build with MinGW
----------------
This project may be built using MinGW as well. Here are the steps to
install it.

  1. Download mingw-get-setup.exe from
     <http://sourceforge.net/projects/mingw/>.
     It is available at Files > Installer.

  2. Double click the downloaded file to run it.

  3. Add C:\MinGW\bin to the Windows PATH environment variable.

  4. When MinGW Installation Manager appears, close it.

  5. Enter the following command to install the compiler.

        mingw-get install mingw32-base

Once installed, run `mingwbuild.cmd` available in this directory to
build the project with gcc.

An executable file named `uncap.exe` is generated in the current
directory if the build succeeds.

Version 0.2.2 of Uncap was built using MinGW.


Build with Microsoft Visual Studio 2005
---------------------------------------
Microsoft Visual Studio 2005 is proprietary software, so it needs to be
purchased and installed.

Once installed, run `vs2005build.cmd` available in this directory to
build the project with Microsoft Visual Studio 2005.

An executable file named `uncap.exe` is generated in the current
directory if the build succeeds.

Versions 0.1.0, 0.2.0 and 0.2.1 of Uncap were built using Microsoft
Visual Studio 2005.


Inline documentation
--------------------
The C source code for this project contains inline documentation in the
form of special comment blocks meant to be processed by Doxygen that can
generate documentation automatically. This may be useful in
understanding the source code.

Download Doxygen from
<http://www.stack.nl/~dimitri/doxygen/download.html> and install it. It
is sufficient to download the doxygen binary in a zip, unzip it and copy
doxygen.exe and libclang.dll to a directory that is specified in the
Windows PATH environment variable.

Once installed, change current directory to the top level directory of
this project and then enter the following command.

    doxygen

The generated documentation can be found at html\index.html relative to
the current directory.


Release
-------
The following tasks need to be performed for every release of a new
version. These tasks should be performed with the project's top-level
directory as the current directory.

  - Update copyright notice in LICENSE.md.
  - Update copyright notice in uncap.c.
  - Update `COPYRIGHT` in uncap.c.
  - Update `VERSION` in uncap.c.
  - Update version in `DOWNLOAD` URL in README.md.
  - Update `PROJECT_NUMBER` in Doxyfile.
  - Update CHANGES.md.
  - Build Uncap.

        dev\clean.cmd
        doxygen
        dev\mingwbuild.cmd
        sha1sum uncap.exe > uncap.sha1

  - Run `uncap -v` and ensure the version and copyright notice are okay.
  - Tag the release.

        git tag -a <VERSION> -m "Uncap <VERSION>"
        git push origin <VERSION>

  - Upload uncap.exe and uncap.sha1 to GitHub release page.
  - Download uncap.exe and uncap.sha1 from GitHub release page and
    verify checksum.

        sha1sum -c uncap.sha1
