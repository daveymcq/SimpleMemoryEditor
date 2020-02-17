@echo off
windres ".\data\resource.rc" --target=pe-i386 -o ".\data\res32.o"
windres ".\data\resource.rc" --target=pe-x86-64 -o ".\data\res64.o"
g++ ".\src\main.cpp" ".\data\res32.o" -o ".\bin\x86\Simple Memory Editor (32-bit).exe" -lcomctl32 -lgdi32 -mwindows -m32 -static
g++ ".\src\main.cpp" ".\data\res64.o" -o ".\bin\x64\Simple Memory Editor (64-bit).exe" -lcomctl32 -lgdi32 -mwindows -m64 -static
g++ ".\src\test.cpp" -o ".\bin\x86\test_x86.exe" -g -static -m32 -Os
g++ ".\src\test.cpp" -o ".\bin\x64\test_x64.exe" -g -static -m64 -Os
echo Done!
pause
