@echo off

set VCVARS=%VCVARS%

set COMPILE_32BIT=yes
set COMPILE_64BIT=yes

if not [%1] == [] (
    set VCVARS=%1%
    goto :compile
)


:compile

if defined VCVARS (
    if %COMPILE_64BIT% == yes (
    
        call %VCVARS% x64>NUL
        mkdir bin\x64
        rc ".\resources\resource.rc"
        cl ".\src\main.cpp" ".\resources\resource.res" /MT /link /subsystem:windows,5.02 /out:".\bin\x64\Simple Memory Editor (64-bit).exe"
        del ".\bin\x64\*.manifest"
        del ".\*.obj"
        
    ) else (
        echo Skipping 64-bit compilation.
    )
)

if defined VCVARS (
    if %COMPILE_32BIT% == yes (
    
        call %VCVARS% x86>NUL
        mkdir bin\x86
        rc ".\resources\resource.rc"
        cl ".\src\main.cpp" ".\resources\resource.res" /MT /link /subsystem:windows,5.02 /out:".\bin\x86\Simple Memory Editor (32-bit).exe"
        del ".\bin\x86\*.manifest"
        del ".\*.obj"
        
    ) else (
        echo Skipping 32-bit compilation.
    )
)