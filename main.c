/* Memory Editing Application
 Author: David McHugh Jr.
 Last Modified: 03/15/2021 */

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WINXP

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#include <windows.h>
#include <tlhelp32.h>
#include <commctrl.h>

#include "src/initialize.h"

/* Entry point. */

int32 main(int32 argc, string argv[])
{
    if(Initialize())
    {
        if(CreateMemoryScannerWindow())
        {
            MSG message;

            while(GetMessage(&message, null, 0, 0) > 0)
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }

            return (int32)message.wParam;
        }
    }

    return (int32)MessageBoxA(null, "The application failed to start.", Title, MB_OK | MB_ICONERROR);
}
