#!/bin/sh
i686-w64-mingw32-windres "./data/resource.rc" --target=pe-i386 -o "./data/res32.o"
x86_64-w64-mingw32-windres "./data/resource.rc" --target=pe-x86-64 -o "./data/res64.o"
i686-w64-mingw32-g++ "./src/main.cpp" "./data/res32.o" -o "./bin/x86/Simple Memory Editor (32-bit).exe" -lcomctl32 -lgdi32 -mwindows -m32 -static
x86_64-w64-mingw32-g++ "./src/main.cpp" "./data/res64.o" -o "./bin/x64/Simple Memory Editor (64-bit).exe" -lcomctl32 -lgdi32 -mwindows -m64 -static
i686-w64-mingw32-gcc "./src/test.cpp" -o "./bin/x86/test_x86.exe" -g -static -m32 -Os
x86_64-w64-mingw32-gcc "./src/test.cpp" -o "./bin/x64/test_x64.exe" -g -static -m64 -Os
rm -f .* 2> /dev/null
rm -f ./src/.* 2> /dev/null
rm -f ./data/.* 2> /dev/null
echo Done!
