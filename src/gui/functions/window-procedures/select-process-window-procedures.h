#ifndef _SELECT_PROCESS_WINDOW_PROCEDURES_H
#define _SELECT_PROCESS_WINDOW_PROCEDURES_H

/* ************************************************************************************ */

/* ************************************************************************************ */

// The window procedure for the process selection dialog.

LRESULT CALLBACK ChooseProcessWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    LPDRAWITEMSTRUCT pdis;

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

        case WM_PAINT:

            hdc = BeginPaint(hWnd, &ps);
            PaintCustomWindowFrame(hWnd, hdc, ChooseProcessWindowWidth, ChooseProcessWindowHeight);
            EndPaint(hWnd, &ps);

        break;

        case WM_SIZE:

            ChooseProcessWindowWidth = LOWORD(lParam);
            ChooseProcessWindowHeight = HIWORD(lParam);

        break;

        case WM_DRAWITEM:

            pdis = (LPDRAWITEMSTRUCT)lParam;

            switch(pdis->CtlID)
            {
                case ID_CLOSE_CHOOSE_PROCESS:

                    DrawCloseButton(hWnd, pdis->hDC);

                break;
            }

        break;

        case WM_NCHITTEST:

            return UpdateWindowLocation(hWnd, Msg, wParam, lParam);

        break;

        case WM_DESTROY:

            EnableWindow(MainWindow, true);
            ShowWindow(MainWindow, SW_SHOW);

        break;

        case WM_COMMAND:

            if(LOWORD(wParam) == ID_CLOSE_CHOOSE_PROCESS)
            {
                DestroyWindow(hWnd);
            }

            else if(LOWORD(wParam) == ID_CHOOSE_PROCESS)
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

        break;
    }

    return 0;
}

#endif


