@echo off

mkdir bin\x86
mkdir bin\x64

call %VC64%
rc ".\data\resource.rc"
cl ".\src\main.cpp" ".\data\resource.res" /MT /link /subsystem:windows,5.02 /out:".\bin\x64\Simple Memory Editor (64-bit).exe"

call %VC32%
rc ".\data\resource.rc"
cl ".\src\main.cpp" ".\data\resource.res" /MT /link /subsystem:windows,5.02 /out:".\bin\x86\Simple Memory Editor (32-bit).exe"

del ".\*.obj"
del ".\bin\x64\*.manifest"
del ".\bin\x86\*.manifest"

