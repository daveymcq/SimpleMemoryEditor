@echo off

set VCVARS=

echo Compiling for x86...

call "%VCVARS%\vcvarsall.bat" x86>NUL

mkdir "bin\x86" 2>NUL
rc "resources\resource.rc"
cl "main.c*" "resources\resource.res" /MT /link /entry:mainCRTStartup /nodefaultlib:libc  /subsystem:windows,4.02 /out:"bin\x86\Simple Memory Editor (32-bit).exe"
del "bin\x86\*.manifest" 2>NUL
del "*.obj" 2>NUL
del "resources\resource.res" 2>NUL

echo Compiling for x64...

call "%VCVARS%\vcvarsall.bat" x64>NUL

mkdir "bin\x64" 2>NUL
rc "resources\resource.rc"
cl "main.c*" "resources\resource.res" /MT /link /entry:mainCRTStartup /nodefaultlib:libc  /subsystem:windows,5.02 /out:"bin\x64\Simple Memory Editor (64-bit).exe"
del "bin\x64\*.manifest" 2>NUL
del "*.obj" 2>NUL
del "resources\resource.res" 2>NUL