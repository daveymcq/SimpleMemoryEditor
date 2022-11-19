#ifndef _CHANGE_VALUE_WINDOW_PROCEDURE_H
#define _CHANGE_VALUE_WINDOW_PROCEDURE_H

/* ************************************************************************************ */

/* ************************************************************************************ */

/* The window procedure for the dialog that modifies the value of a scan result. */

LRESULT CALLBACK ChangeValueWindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch(message)
    {
        case WM_CREATE:
        break;

        case WM_CLOSE:

            DestroyWindow(window);

        break;

        case WM_DESTROY:

            EnableWindow(MemoryScannerWindow, true);
            ShowWindow(MemoryScannerWindow, SW_SHOW);
            CenterWindow(MemoryScannerWindow);
            SetForegroundWindow(MemoryScannerWindow);

        break;

        case WM_COMMAND:

            if(LOWORD(wparam) == ID_CHANGE_VALUE_WINDOW_BUTTON)
            {
                UpdateValue();
            }

        break;

        case WM_DPICHANGED:

            UpdateLayoutForDpi(ChangeValueWindow, CW_USEDEFAULT, CW_USEDEFAULT, 300, 75);
            UpdateLayoutForDpi(ChangeValueWindowNewValue, 10, 10, 180, 25);
            UpdateLayoutForDpi(ChangeValueWindowButton, 200, 10, 85, 25);

            ShowWindow(MemoryScannerWindow, SW_HIDE);
            CenterWindow(ChangeValueWindow);

        break;

        default:

            return DefWindowProc(window, message, wparam, lparam);
    }

    return 0;
}

#endif


