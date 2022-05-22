@echo off

setlocal EnableDelayedExpansion

set /a vs_directories=0

for /d %%a in ("%programfiles%\Microsoft Visual Studio*") do (

    for /f "tokens=3 delims=\" %%x in ("%%a") do (

        if !vs_directories! gtr 0 echo | set /p=,

        set /a vs_directories+=1 
        echo | set /p=%%a
    )
)

for /d %%a in ("%systemdrive%\Program Files (x86)\Microsoft Visual Studio*") do (

    for /f "tokens=3 delims=\" %%x in ("%%a") do (

        if !vs_directories! gtr 0 echo | set /p=,

        set /a vs_directories+=1 
        echo | set /p=%%a
    )
)
