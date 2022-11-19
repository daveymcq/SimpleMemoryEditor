/* Memory Editing Application
 Author: David McHugh Jr.
 Last Modified: 11/19/2022 */

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WINXP

#include <windows.h>
#include <tlhelp32.h>
#include <commctrl.h>

#include "initialize.h"

/* Entry point. */

int32 main(int32 argc, string argv[])
{
    if(Initialize(GetModuleHandle(null)))
    {
        if(CreateMemoryScannerWindow())
        {
            while(GetMemoryScannerWindowMessages());
        }
    }

    return 0;
}
