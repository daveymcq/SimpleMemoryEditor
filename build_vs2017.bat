@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
rc ".\data\resource.rc"
cl ".\src\main.cpp" ".\data\resource.res" /MT /link /subsystem:windows,5.02 /out:".\bin\x64\Simple Memory Editor (64-bit).exe"
cl ".\src\test.cpp" /link /subsystem:console,5.02 /out:".\bin\x64\test_x64.exe"
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
cl ".\src\main.cpp" ".\data\resource.res" /MT /link /subsystem:windows,5.01 /out:".\bin\x86\Simple Memory Editor (32-bit).exe"
cl ".\src\test.cpp" /link /subsystem:console,5.01 /out:".\bin\x86\test_x86.exe"
del ".\*.obj"
del ".\bin\x64\*.manifest"
del ".\bin\x86\*.manifest"
echo Done!
pause
