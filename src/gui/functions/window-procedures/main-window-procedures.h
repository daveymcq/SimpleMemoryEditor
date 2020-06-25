#ifndef _MAIN_WINDOW_PROCEDURES_H
#define _MAIN_WINDOW_PROCEDURES_H

/* ************************************************************************************ */

/* ************************************************************************************ */

// The window procedure for the main window.

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    LPDRAWITEMSTRUCT pdis;

    switch(Msg)
    {
        case WM_CREATE:

            CreateMainWindowUI(hWnd);
            CenterWindow(hWnd);

        break;

        case WM_CLOSE:

            ProcessMainWindowCloseEvent(hWnd);

        break;

        case WM_NCHITTEST:

            return UpdateWindowLocation(hWnd, Msg, wParam, lParam);

        break;

        case WM_PAINT:

            hdc = BeginPaint(hWnd, &ps);
            PaintCustomWindowFrame(hWnd, hdc, MainWindowWidth, MainWindowHeight);
            EndPaint(hWnd, &ps);

        break;

        case WM_SIZE:

            MainWindowWidth = LOWORD(lParam);
            MainWindowHeight = HIWORD(lParam);

        break;

        case WM_DESTROY:

            PostQuitMessage(0);

        break;

        case WM_NOTIFY:

            if(!ScanRunning)
            {
                if(((LPNMHDR)lParam)->code == NM_CLICK)
                {
                    ProcessListViewLeftClickEvent();
                }

                else if((((LPNMHDR)lParam)->code == NM_RCLICK) && (((LPNMHDR)lParam)->idFrom == ID_LISTVIEW))
                {
                    ProcessListViewRightClickEvent(hWnd);
                }
            }

        break;

        case WM_DRAWITEM:

            pdis = (LPDRAWITEMSTRUCT)lParam;

            switch(pdis->CtlID)
            {
                case ID_CLOSE:

                    DrawCloseButton(hWnd, pdis->hDC, 13, 4, 34, CustomToolbarHeight - 6);

                break;

                case ID_SELECT_PROCESS:

                    DrawCustomButton(hWnd, pdis->hDC, ID_SELECT_PROCESS, 12, 5, 100, 25);

                break;

                case ID_SCAN:

                    DrawCustomButton(hWnd, pdis->hDC, ID_SCAN, 270, 15, 600, 50);
                    
                break;

                case ID_NEW_SCAN:

                    DrawCustomButton(hWnd, pdis->hDC, ID_NEW_SCAN, 20, 5, 100, 25);

                break;

                case ID_CHANGE_VALUE:

                    DrawCustomButton(hWnd, pdis->hDC, ID_CHANGE_VALUE, 12, 5, 100, 25);

                break;
            }

            break;

        case WM_COMMAND:

            if(LOWORD(wParam) == ID_CLOSE)
            {
                PostQuitMessage(0);
            }

            else if((LOWORD(wParam) == ID_FREEZE_VALUE) && (SelectedItem != -1))
            {
                ProcessFreezeValueButtonEvent();
            }

            else if((LOWORD(wParam) == ID_UNFREEZE_VALUE) && (SelectedItem != -1))
            {
                ProcessUnfreezeValueButtonEvent();
            }

            else if(LOWORD(wParam) == ID_SCAN)
            {
                TerminateThread(ScanThread, 0);
                CloseHandle(ScanThread);

                DWORD ThreadID;

                ScanThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ProcessScan, 0, 0, &ThreadID);
            }

            else if(LOWORD(wParam) == ID_SELECT_PROCESS)
            {
                GetProcessNameAndID();
                CreateChooseProcessWindow();
            }

            else if(LOWORD(wParam) == ID_CHANGE_VALUE)
            {
                if((!SelectedAddressFrozen()) && (GetMatchCount(scanner))) CreateChangeValueDialog();
            }

            else if(LOWORD(wParam) == ID_NEW_SCAN)
            {
                if(scanner) ResetScan(scanner, false, true);
            }

            break;

        default:

            return DefWindowProc(hWnd, Msg, wParam, lParam);

        break;
    }

    return 0;
}

#endif

