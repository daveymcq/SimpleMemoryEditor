rm -rf 'bin' 2> /dev/null

mkdir 'bin' 2> /dev/null
mkdir 'bin/x64' 2> /dev/null
mkdir 'bin/x86' 2> /dev/null

i686-w64-mingw32-windres "src/resources/icon.rc" -o "src/resources/resources.o" --target=pe-i386
i686-w64-mingw32-g++ "main.c" "src/resources/resources.o" -static -mwindows -lgdi32 -lcomctl32 -Wall -o "bin/x86/Simple Memory Editor (32-bit).exe"
rm -f "resources/*.o" > /dev/null

x86_64-w64-mingw32-windres "src/resources/icon.rc" -o "src/resources/resources.o" --target=pe-x86-64
x86_64-w64-mingw32-g++ "main.c" "src/resources/resources.o" -static -mwindows -lgdi32 -lcomctl32 -Wall -o "bin/x64/Simple Memory Editor (64-bit).exe"
rm -f "resources/*.o" > /dev/null