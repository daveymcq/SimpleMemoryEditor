@echo off

if [%1] == [] (

    echo 'ERROR'
  
) else (

    pushd build

    echo .....................................................................................................
    echo Compiling for x86
    echo .....................................................................................................
    
    call %1 x86>NUL

    mkdir "..\bin\x86" 2>NUL
    rc "..\src\resources\icon.rc"
    cl "..\src\main.c*" "..\src\resources\icon.res" /MT /link /nodefaultlib:vcruntime.lib /entry:mainCRTStartup /subsystem:windows,4.02 /out:"..\bin\x86\Simple Memory Editor (32-bit).exe"
    del "..\src\bin\x86\*.manifest" 2>NUL
    del "..\src\resources\*.res" 2>NUL
    del "*.obj" 2>NUL

    echo .....................................................................................................
    echo Compiling for x64
    echo .....................................................................................................
    
    call %1 x64>NUL

    mkdir "..\bin\x64" 2>NUL
    rc "..\src\resources\icon.rc"
    cl "..\src\main.c*" "..\src\resources\icon.res" /MT /link /nodefaultlib:vcruntime.lib /entry:mainCRTStartup /subsystem:windows,5.02 /out:"..\bin\x64\Simple Memory Editor (64-bit).exe"
    del "..\src\bin\x64\*.manifest" 2>NUL
    del "..\src\resources\*.res" 2>NUL
    del "*.obj" 2>NUL
    
    echo .....................................................................................................

    popd
)