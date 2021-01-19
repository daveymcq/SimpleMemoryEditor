#ifndef _SELECT_PROCESS_WINDOW_PROCEDURES_H
#define _SELECT_PROCESS_WINDOW_PROCEDURES_H

/* ************************************************************************************ */

/* ************************************************************************************ */

// The window procedure for the process selection dialog.

LRESULT CALLBACK SelectProcessWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
        case WM_CLOSE:

            DestroyWindow(hWnd);
            SetForegroundWindow(MainWindow);

        break;

        case WM_CREATE:

            ShowWindow(MainWindow, SW_HIDE);
            CenterWindow(hWnd);

        break;

        case WM_DESTROY:

            EnableWindow(MainWindow, true);
            ShowWindow(MainWindow, SW_SHOW);
            SetForegroundWindow(MainWindow);

        break;

        case WM_COMMAND:

            if(LOWORD(wParam) == ID_CHOOSE_PROCESS)
            {
                ProcessSelectProcessButtonEvent();
            }

            else if((LOWORD(wParam) == ID_PROCESSES) && (HIWORD(wParam) == LBN_SELCHANGE))
            {
                ProcessListboxChangeEvent();
            }

        break;

        default:

            return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    return 0;
}

#endif


