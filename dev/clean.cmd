@echo off
setlocal

rem Top level directory of the project.
set top=%~dp0..

rd /s /q %top%\html
rd /s /q %top%\latex
del %top%\uncap.exe
del %top%\uncap.obj
del %top%\uncap.sha1
