#ifndef _SELECT_PROCESS_WINDOW_PROCEDURE_H
#define _SELECT_PROCESS_WINDOW_PROCEDURE_H

/* ************************************************************************************ */

/* ************************************************************************************ */

/* The window procedure for the process selection dialog. */

LRESULT CALLBACK SelectProcessWindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch(message)
    {
        case WM_CREATE:
        break;

        case WM_CLOSE:

            DestroyWindow(window);

        break;

        case WM_DESTROY:
            
            ShowWindow(MemoryScannerWindow, SW_SHOW);
            SetForegroundWindow(MemoryScannerWindow);
            EnableWindow(MemoryScannerWindow, true);
            CenterWindow(MemoryScannerWindow);

        break;

        case WM_COMMAND:

            if(LOWORD(wparam) == ID_CHOOSE_PROCESS)
            {
                ProcessSelectProcessButtonEvent();
            }

            else if((LOWORD(wparam) == ID_PROCESSES) && (HIWORD(wparam) == LBN_SELCHANGE))
            {
                ProcessListboxChangeEvent();
            }

        break;

        default:

            return DefWindowProc(window, message, wparam, lparam);
    }

    return 0;
}

#endif


