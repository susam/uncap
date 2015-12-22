@echo off
setlocal

rem Set environment for Visual Studio 2005 tools.
call "%vs80comntools%\vsvars32.bat"

rem KBDLLHOOKSTRUCT, LLKHF_EXTENDED, etc. defined in winuser.h requires
rem _WIN32_WINNT >= 0x0400.

rem PKEYBDINPUT, INPUT, SendInput, etc. defined in winuser.h requires
rem _WIN32_WINNT > 0x0400

rem Build uncap.
cl /W4 /O2 /D_CRT_SECURE_NO_WARNINGS /D_WIN32_WINNT=0x0401 uncap.c /link user32.lib
