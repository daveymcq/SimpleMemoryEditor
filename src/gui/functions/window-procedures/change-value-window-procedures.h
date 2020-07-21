#ifndef _CHANGE_VALUE_WINDOW_PROCEDURES_H
#define _CHANGE_VALUE_WINDOW_PROCEDURES_H

/* ************************************************************************************ */

/* ************************************************************************************ */

// The window procedure for the dialog that modifies the value of a scan result.

LRESULT CALLBACK ChangeValueWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
        case WM_DESTROY:

            EnableWindow(MainWindow, true);
            ShowWindow(MainWindow, SW_SHOW);
            SetForegroundWindow(MainWindow);

        break;

        case WM_CREATE:

            ShowWindow(MainWindow, SW_HIDE);
            CenterWindow(hWnd);

        break;

        case WM_COMMAND:

            if(LOWORD(wParam) == ID_CHANGE_DLG_BUTTON)
            {
                UpdateValue();
            }

        break;

        default:

            return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    return 0;
}

#endif


