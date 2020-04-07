/* Memory Editing Application
 Author: David McHugh Jr.
 Last Modified: 12/02/2019 */

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

#include "lib/mcqlibc.h"
#include "global_variables.h"
#include "core_functions.h"
#include "gui.h"
#include "window_procs.h"

// Entry point.

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    if(Initialize())
    {
        WNDCLASSEX wc;

        wc.cbSize = sizeof(wc);
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
        wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
        wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(AppIcon));
        wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(AppIcon));
        wc.hInstance = hInstance;
        wc.lpfnWndProc = MainWndProc;
        wc.lpszClassName = "Main";
        wc.lpszMenuName = 0;
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

        if(RegisterClassEx(&wc))
        {
            MainWindow = CreateWindowEx(WS_EX_STATICEDGE, wc.lpszClassName, title,
                                        WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, 
                                        CW_USEDEFAULT, Width, Height, 0, 0, hInstance, 0);
            if(MainWindow)
            {
                ShowWindow(MainWindow, nCmdShow);
                UpdateWindow(MainWindow);
                 
                MSG Msg;

                while(GetMessage(&Msg, 0, 0, 0) > 0)
                {
                    TranslateMessage(&Msg);
                    DispatchMessage(&Msg);
                }

                return (int)Msg.wParam;
            }
        }
    }

    MessageBox(0, "The application failed to start.", title, MB_OK | MB_ICONERROR);
    return -1;
}
