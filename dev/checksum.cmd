@echo off

certutil -hashfile %~dp0..\uncap.exe sha256 | ^
findstr /v "^SHA256 ^CertUtil" > uncap.sha256

type uncap.sha256
