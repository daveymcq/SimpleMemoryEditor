#ifndef _CHANGE_VALUE_WINDOW_PROCEDURES_H
#define _CHANGE_VALUE_WINDOW_PROCEDURES_H

/* ************************************************************************************ */

/* ************************************************************************************ */

// The window procedure for the dialog that modifies the value of a scan result.

LRESULT CALLBACK ChangeValueWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{

    HDC hdc;
    PAINTSTRUCT ps;
    LPDRAWITEMSTRUCT pdis;

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

        case WM_SIZE:

            ChangeValueWindowWidth = LOWORD(lParam);
            ChangeValueWindowHeight = HIWORD(lParam);

        break;

        case WM_PAINT:

            hdc = BeginPaint(hWnd, &ps);
            PaintCustomWindowFrame(hWnd, hdc, ChangeValueWindowWidth, ChangeValueWindowHeight);     
            EndPaint(hWnd, &ps);

        break;

        case WM_DRAWITEM:

            pdis = (LPDRAWITEMSTRUCT)lParam; 

            switch(pdis->CtlID)
            {
                case ID_CLOSE_CHANGE_VALUE:

                    DrawCloseButton(hWnd, pdis->hDC, 13, 4, 34, CustomToolbarHeight - 6); 

                break;

                case ID_CHANGE_DLG_BUTTON:

                    DrawCustomButton(hWnd, pdis->hDC, ID_CHANGE_DLG_BUTTON, 20, 5, 85, 25);

                break;
            }

        break;

        case WM_NCHITTEST:

            return UpdateWindowLocation(hWnd, Msg, wParam, lParam);

        break;

        case WM_COMMAND:

            if(LOWORD(wParam) == ID_CLOSE_CHANGE_VALUE)
            {
                DestroyWindow(hWnd);
            }

            else if(LOWORD(wParam) == ID_CHANGE_DLG_BUTTON)
            {
                ProcessUpdateValueEvent();
            }

        break;

        default:

            return DefWindowProc(hWnd, Msg, wParam, lParam);

        break;
    }

    return 0;
}

#endif


