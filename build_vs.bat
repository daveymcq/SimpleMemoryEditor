@echo off

mkdir bin\x86>NUL
mkdir bin\x64>NUL

@%VC64%
rc ".\data\resource.rc"
cl ".\src\main.c" ".\data\resource.res" /MT /link /subsystem:windows,5.02 /out:".\bin\x64\Simple Memory Editor (64-bit).exe"

@%VC32%
rc ".\data\resource.rc"
cl ".\src\main.c" ".\data\resource.res" /MT /link /subsystem:windows,5.02 /out:".\bin\x86\Simple Memory Editor (32-bit).exe"

del ".\*.obj"
del ".\bin\x64\*.manifest"
del ".\bin\x86\*.manifest"

echo Done!
