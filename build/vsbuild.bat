@echo off

echo Compiling for x86...
echo .

set VCVARS=%1\vcvarsall.bat

call %VCVARS% x86>NUL

mkdir "..\bin\x86" 2>NUL
rc "..\resources\icon.rc"
cl "..\main.c*" "..\resources\icon.res" /MT /link /entry:mainCRTStartup /nodefaultlib:vcruntime.lib  /subsystem:windows,4.02 /out:"..\bin\x86\Simple Memory Editor (32-bit).exe"
del "..\bin\x86\*.manifest" 2>NUL
del "..\resources\*.res" 2>NUL
del "*.obj" 2>NUL

echo .
echo Compiling for x64...

call %VCVARS% x64>NUL

mkdir "..\bin\x64" 2>NUL
rc "..\resources\icon.rc"
cl "..\main.c*" "..\resources\icon.res" /MT /link /entry:mainCRTStartup /nodefaultlib:vcruntime.lib  /subsystem:windows,5.02 /out:"..\bin\x64\Simple Memory Editor (64-bit).exe"
del "..\bin\x64\*.manifest" 2>NUL
del "..\resources\*.res" 2>NUL
del "*.obj" 2>NUL