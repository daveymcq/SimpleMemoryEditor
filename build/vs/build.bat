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
    cl "..\src\main.c" "..\src\resources\icon.res"  /W0 /Zl /O1 /Oi /MT /GR- /GS- /Gs999999999 /EHa- /fp:except- /fp:fast /nologo /link /incremental:no /stack:0x100000,0x100000 /nodefaultlib /opt:ref /opt:icf /merge:.rdata=.text /entry:Main /subsystem:windows,5.01 /out:"..\bin\x86\Simple Memory Editor (32-bit).exe" comctl32.lib gdi32.lib user32.lib kernel32.lib 
    del "..\src\resources\*.res" 2>NUL
    del "..\bin\x86\*.manifest" 2>NUL
    del "*.obj" 2>NUL

    echo .....................................................................................................
    echo Compiling for x64
    echo .....................................................................................................
    
    call %1 x64>NUL

    mkdir "..\bin\x64" 2>NUL
    rc "..\src\resources\icon.rc"
    cl "..\src\main.c" "..\src\resources\icon.res" /W0 /Zl /O1 /MT /Oi /GR- /GS- /Gs999999999 /EHa- /fp:except- /fp:fast /nologo /link /incremental:no /stack:0x100000,0x100000 /nodefaultlib /opt:ref /opt:icf /merge:.rdata=.text /entry:Main /subsystem:windows,5.02 /out:"..\bin\x64\Simple Memory Editor (64-bit).exe" comctl32.lib gdi32.lib user32.lib kernel32.lib 
    del "..\src\resources\*.res" 2>NUL
    del "..\bin\x64\*.manifest" 2>NUL
    del "*.obj" 2>NUL
    
    echo .....................................................................................................

    popd
)