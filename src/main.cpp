/* Memory Editing Application
 Author: David McHugh Jr.
 Last Modified: 06/25/2020 */

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#include <windows.h>
#include <tlhelp32.h>
#include <commctrl.h>

#include "resource.h"
#include "declarations.h"
#include "foundation/functions/lowlevelfunctions/low-level-functions.h"
#include "scanner/functions/scanner-functions.h"
#include "gui/functions/gui-functions.h"

// Entry point.

int main(int argc, char **argv)
{
    if(Initialize())
    {
        return CreateMainWindow();
    }

    MessageBox(0, "The application failed to start.", title, MB_OK | MB_ICONERROR);

    return -1;
}
