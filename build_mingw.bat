@echo off

mkdir bin\x86
mkdir bin\x64

windres ".\data\resource.rc" --target=pe-i386 -o ".\data\res32.o"
windres ".\data\resource.rc" --target=pe-x86-64 -o ".\data\res64.o"

g++ ".\src\main.cpp" ".\data\res32.o" -o ".\bin\x86\Simple Memory Editor (32-bit).exe" -lcomctl32 -lgdi32 -mwindows -m32 -static
g++ ".\src\main.cpp" ".\data\res64.o" -o ".\bin\x64\Simple Memory Editor (64-bit).exe" -lcomctl32 -lgdi32 -mwindows -m64 -static
