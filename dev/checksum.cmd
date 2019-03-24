@echo off
setlocal

certutil -hashfile %~dp0..\uncap.exe sha256 | ^
findstr /v "^SHA256 ^CertUtil" > uncap.sha256.txt

set /p hash= < uncap.sha256.txt
> uncap.sha256.txt echo %hash% *uncap.exe
type uncap.sha256.txt
