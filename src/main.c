/* Memory Editing Application
 Author: David McHugh Jr.
 Last Modified: 04/08/2025 */

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WINXP

#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "ComCtl32.lib")

#include <windows.h>
#include <tlhelp32.h>
#include <commctrl.h>

#include "initialize.h"

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

    return -1;
}
