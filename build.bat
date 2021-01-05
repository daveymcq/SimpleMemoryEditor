@echo off

:choose_compiler

set /p COMPILER=GCC or Visual Studio? (enter 'gcc' or 'vs') : 
set VALID_OPTION=

if /i %COMPILER% == vs (
    set VALID_OPTION=true
)

if /i %COMPILER% == gcc (
    set VALID_OPTION=true
)

if defined VALID_OPTION (
    goto choose_compile_32bit
)

goto choose_compiler

:choose_compile_32bit

set /p COMPILE_32BIT=Compile 32-bit binary? (yes/no) : 
set VALID_OPTION=

if /i %COMPILE_32BIT% == yes (
    set VALID_OPTION=true
)

if /i %COMPILE_32BIT% == no (
    set VALID_OPTION=true
)

if defined VALID_OPTION (
    goto choose_compile_64bit
)

goto choose_compile_32bit

:choose_compile_64bit

set /p COMPILE_64BIT=Compile 64-bit binary? (yes/no) : 
set VALID_OPTION=

if /i %COMPILE_64BIT% == yes (
    set VALID_OPTION=true
)

if /i %COMPILE_64BIT% == no (
    set VALID_OPTION=true
)

if defined VALID_OPTION (
    goto compiler_check
)

goto choose_compile_64bit

:compiler_check

if /i %COMPILER% == vs (

    cls
    
    if defined VS80COMNTOOLS (
        echo Using Visual Studio 2005.
        set VCVARS="%VS80COMNTOOLS:~0,-14%VC\vcvarsall.bat"
        goto compile_vs
    ) else (
        
        if defined VS90COMNTOOLS (
            echo Using Visual Studio 2008.
            set VCVARS="%VS90COMNTOOLS:~0,-14%VC\vcvarsall.bat"
            goto compile_vs
        ) else (
        
            if defined VS100COMNTOOLS (
                echo Using Visual Studio 2010.
                set VCVARS="%VS100COMNTOOLS:~0,-14%VC\vcvarsall.bat"
                goto compile_vs
            )
        )
    )
    
    goto no_compiler
)

if /i %COMPILER% == gcc (
    cls
    goto compile_gcc
)

:compile_vs

if defined VCVARS (
    if %COMPILE_64BIT% == yes (
    
        echo building for x64
    
        call %VCVARS% x64>NUL
        mkdir bin\x64
        rc ".\resources\resource.rc"
        cl ".\src\main.cpp" ".\resources\resource.res" /MT /link /entry:mainCRTStartup /nodefaultlib:libc  /subsystem:windows,5.02 /out:".\bin\x64\Simple Memory Editor (64-bit).exe"
        del ".\bin\x64\*.manifest"
        del ".\*.obj"
        del ".\resources\resource.res"
        
    ) else (
        echo Skipping 64-bit compilation.
    )
)

if defined VCVARS (
    if %COMPILE_32BIT% == yes (
        
        echo building for x86
        
        call %VCVARS% x86>NUL
        mkdir bin\x86
        rc ".\resources\resource.rc"
        cl ".\src\main.c*" ".\resources\resource.res" /MT /link /entry:mainCRTStartup /nodefaultlib:libc /subsystem:windows,4.02 /out:".\bin\x86\Simple Memory Editor (32-bit).exe"
        del ".\bin\x86\*.manifest"
        del ".\*.obj"
        del ".\resources\resource.res"
        
    ) else (
        echo Skipping 32-bit compilation.
    )
)

echo done!
pause
exit

:compile_gcc

if %COMPILE_64BIT% == yes (
    
    echo building for x64
    
    mkdir bin\x64
    windres ".\resources\resource.rc" -o ".\resources\resource.o" --target=pe-x86-64 
    g++ ".\src\main.c*" ".\resources\resource.o" -static -mwindows -o ".\bin\x64\Simple Memory Editor (64-bit).exe" -lcomctl32
    del ".\resources\resource.o"
        
) else (
    echo Skipping 64-bit compilation.
)

if %COMPILE_32BIT% == yes (
    
    echo building for x86
    
    mkdir bin\x86
    windres ".\resources\resource.rc" -o ".\resources\resource.o" --target=pe-i386
    g++ ".\src\main.cpp" ".\resources\resource.o" -static -mwindows -m32 -o ".\bin\x86\Simple Memory Editor (32-bit).exe" -lcomctl32
    del ".\resources\resource.o"
        
) else (
    echo Skipping 32-bit compilation.
)

echo done!
pause
exit

:no_compiler

set /p VCVARS_INPUT=No Visual studio compiler was found on this machine. Enter the path to vcvarsall.bat, or just press enter to exit. : 

if not %VCVARS_INPUT% == "" (
    set VCVARS=%VCVARS_INPUT%
    goto :compile_vs
)