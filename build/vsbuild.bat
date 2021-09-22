@echo off

if [%1] == [] (

    echo Error: Provide the directory that contains your Visual Studio installation's "vcvarsall.bat" as a command line argument.
  
) else (

    echo .....................................................................................................
    echo Compiling for x86
    echo .....................................................................................................
    
    call %1\vcvarsall.bat x86>NUL

    mkdir "..\bin\x86" 2>NUL
    rc "..\src\resources\icon.rc"
    cl "..\main.c*" "..\src\resources\icon.res" /MT /link /nodefaultlib:vcruntime.lib /entry:mainCRTStartup /subsystem:windows,4.02 /out:"..\bin\x86\Simple Memory Editor (32-bit).exe"
    del "..\bin\x86\*.manifest" 2>NUL
    del "..\src\resources\*.res" 2>NUL
    del "*.obj" 2>NUL

    echo .....................................................................................................
    echo Compiling for x64
    echo .....................................................................................................
    
    call %1\vcvarsall.bat x64>NUL

    mkdir "..\bin\x64" 2>NUL
    rc "..\src\resources\icon.rc"
    cl "..\main.c*" "..\src\resources\icon.res" /MT /link /nodefaultlib:vcruntime.lib /entry:mainCRTStartup /subsystem:windows,5.02 /out:"..\bin\x64\Simple Memory Editor (64-bit).exe"
    del "..\bin\x64\*.manifest" 2>NUL
    del "..\src\resources\*.res" 2>NUL
    del "*.obj" 2>NUL
    
    echo .....................................................................................................
)