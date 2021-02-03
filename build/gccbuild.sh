#!/bin/sh

echo building for x86...
    
mkdir -p "../bin/x86"
i686-w64-mingw32-windres "../resources/icon.rc" -o "../resources/icon.o" --target=pe-i386
i686-w64-mingw32-gcc "../main.c" "../resources/icon.o" -static -mwindows -m32 -lcomctl32 -o "../bin/x86/Simple Memory Editor (32-bit).exe"
rm -f "../resources/*.o"

echo building for x64...

mkdir -p "../bin/x64"
x86_64-w64-mingw32-windres "../resources/icon.rc" -o "../resources/icon.o" --target=pe-x86-64
x86_64-w64-mingw32-gcc "../main.c" "../resources/icon.o" -static -mwindows -lcomctl32 -o "../bin/x64/Simple Memory Editor (64-bit).exe"
rm -f "../resources/*.o"