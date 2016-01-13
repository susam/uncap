@echo off
setlocal

rem Top level directory of the project
set top=%~dp0..

rem PKEYBDINPUT, INPUT, SendInput, etc. defined in winuser.h requires
rem _WIN32_WINNT >= 0x0403
rem GetProcessImageFilename requires _WIN32_WINNT >= 0x0501
gcc -std=c89 -Wall -Wextra -pedantic -O3 -D_WIN32_WINNT=0x501 "%top%\uncap.c" -o uncap.exe
