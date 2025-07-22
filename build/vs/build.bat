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
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    cl "..\src\main.c" "..\src\resources\icon.res" /Zl /O1 /MT /link /opt:ref /opt:icf /merge:.rdata=.text /entry:mainCRTStartup /subsystem:windows,5.01 /out:"..\bin\x86\Simple Memory Editor (32-bit).exe" libcmt.lib comctl32.lib gdi32.lib user32.lib kernel32.lib 
<<<<<<< HEAD
<<<<<<< HEAD
    cl "..\src\main.c" "..\src\resources\icon.res" /W1 /Oy /Zl /O1 /Oi /MT /GR- /GS- /Gs999999999 /EHa- /fp:precise /fp:except- /nologo /link /incremental:no /stack:0x7fffff,0x7fffff /nodefaultlib /verbose:lib /opt:ref /opt:icf /merge:.rdata=.text /entry:Main /subsystem:windows,5.01 /out:"..\bin\x86\Simple Memory Editor (32-bit).exe" comctl32.lib gdi32.lib user32.lib kernel32.lib 
    dumpbin /symbols main.obj | findstr aull
    cl "..\src\main.c" "..\src\resources\icon.res" /Zl /O1 /MT /link /opt:ref /opt:icf /merge:.rdata=.text /entry:mainCRTStartup /subsystem:windows,5.01 /out:"..\bin\x86\Simple Memory Editor (32-bit).exe" libcmt.lib comctl32.lib gdi32.lib user32.lib kernel32.lib 
=======
>>>>>>> parent of 45d2056 (Fix broken backwards compatibility)
=======
>>>>>>> parent of 45d2056 (Fix broken backwards compatibility)
=======
    cl "..\src\main.c" "..\src\resources\icon.res" /Zl /O1 /MT /link /opt:ref /opt:icf /merge:.rdata=.text /entry:mainCRTStartup /subsystem:windows,5.01 /out:"..\bin\x86\Simple Memory Editor (32-bit).exe" libcmt.lib comctl32.lib gdi32.lib user32.lib kernel32.lib 
>>>>>>> parent of 45d2056 (Fix broken backwards compatibility)
    del "..\src\resources\*.res" 2>NUL
=======
    cl "..\src\main.c*" "..\src\resources\icon.res" /Zl /O1 /MT /link libcmt.lib comctl32.lib gdi32.lib user32.lib kernel32.lib /entry:mainCRTStartup /subsystem:windows,5.01 /out:"..\bin\x86\Simple Memory Editor (32-bit).exe"
>>>>>>> parent of 1372378 (Fix broken backwards compatibility)
=======
    cl "..\src\main.c*" "..\src\resources\icon.res" /Zl /O1 /MT /link libcmt.lib comctl32.lib gdi32.lib user32.lib kernel32.lib /entry:mainCRTStartup /subsystem:windows,5.02 /out:"..\bin\x86\Simple Memory Editor (32-bit).exe"
>>>>>>> parent of 3b70bb1 (Fix broken backwards compatibility)
    del "..\bin\x86\*.manifest" 2>NUL
    del "..\src\resources\*.res" 2>NUL
    del "*.obj" 2>NUL

    echo .....................................................................................................
    echo Compiling for x64
    echo .....................................................................................................
    
    call %1 x64>NUL

    mkdir "..\bin\x64" 2>NUL
    rc "..\src\resources\icon.rc"
<<<<<<< HEAD
<<<<<<< HEAD
    cl "..\src\main.c" "..\src\resources\icon.res" /Zl /O1 /MT /link /opt:ref /opt:icf /merge:.rdata=.text /entry:mainCRTStartup /subsystem:windows,5.02 /out:"..\bin\x64\Simple Memory Editor (64-bit).exe" libcmt.lib comctl32.lib gdi32.lib user32.lib kernel32.lib 
<<<<<<< HEAD
<<<<<<< HEAD
    cl "..\src\main.c" "..\src\resources\icon.res" /W1 /Oy /Zl /O1 /MT /Oi /GR- /GS- /Gs999999999 /EHa- /fp:precise /fp:except- /nologo /link /incremental:no /stack:0x7fffff,0x7fffff /nodefaultlib /verbose:lib /opt:ref /opt:icf /merge:.rdata=.text /entry:Main /subsystem:windows,5.02 /out:"..\bin\x64\Simple Memory Editor (64-bit).exe" comctl32.lib gdi32.lib user32.lib kernel32.lib 
    dumpbin /symbols main.obj | findstr aull
    cl "..\src\main.c" "..\src\resources\icon.res" /Zl /O1 /MT /link /opt:ref /opt:icf /merge:.rdata=.text /entry:mainCRTStartup /subsystem:windows,5.02 /out:"..\bin\x64\Simple Memory Editor (64-bit).exe" libcmt.lib comctl32.lib gdi32.lib user32.lib kernel32.lib 
=======
>>>>>>> parent of 45d2056 (Fix broken backwards compatibility)
=======
>>>>>>> parent of 45d2056 (Fix broken backwards compatibility)
=======
    cl "..\src\main.c" "..\src\resources\icon.res" /Zl /O1 /MT /link /opt:ref /opt:icf /merge:.rdata=.text /entry:mainCRTStartup /subsystem:windows,5.02 /out:"..\bin\x64\Simple Memory Editor (64-bit).exe" libcmt.lib comctl32.lib gdi32.lib user32.lib kernel32.lib 
>>>>>>> parent of 45d2056 (Fix broken backwards compatibility)
    del "..\src\resources\*.res" 2>NUL
<<<<<<< HEAD
    del "..\bin\x64\*.manifest" 2>NUL
=======
    cl "..\src\main.c*" "..\src\resources\icon.res" /Zl /O1 /MT /link libcmt.lib comctl32.lib gdi32.lib user32.lib kernel32.lib /entry:mainCRTStartup /subsystem:windows,5.02 /out:"..\bin\x64\Simple Memory Editor (64-bit).exe"
    del "..\bin\x64\*.manifest" 2>NUL
    del "..\src\resources\*.res" 2>NUL
>>>>>>> parent of 1372378 (Fix broken backwards compatibility)
=======
    del "..\bin\x86\*.manifest" 2>NUL
>>>>>>> parent of 48ab795 (Fix broken backwards compatibility)
    del "*.obj" 2>NUL
    
    echo .....................................................................................................

    popd
)