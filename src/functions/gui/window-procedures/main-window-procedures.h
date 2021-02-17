#ifndef _MAIN_WINDOW_PROCEDURES_H
#define _MAIN_WINDOW_PROCEDURES_H

/* ************************************************************************************ */

/* ************************************************************************************ */

// The window procedure for the main window.

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
        case WM_CREATE:

            CenterWindow(hWnd);

        break;

        case WM_CLOSE:

            ProcessMainWindowCloseEvent(hWnd);

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

        case WM_COMMAND:

            if((LOWORD(wParam) == ID_FREEZE_VALUE) && (SelectedItem != -1))
            {
                ProcessFreezeValueButtonEvent();
            }

            else if((LOWORD(wParam) == ID_UNFREEZE_VALUE) && (SelectedItem != -1))
            {
                ProcessUnfreezeValueButtonEvent();
            }

            else if(LOWORD(wParam) == ID_SCAN)
            {
                DWORD ThreadID;

                if(ScanThread && TerminateThread(ScanThread, 0))
                {    
                    DWORD status = WaitForSingleObject(ScanThread, INFINITE);

                    if(status == WAIT_OBJECT_0)
                    {
                        CloseHandle(ScanThread);
                    }
                }

                ScanThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ProcessScan, 0, 0, &ThreadID);
            }

            else if(LOWORD(wParam) == ID_SELECT_PROCESS)
            {
                GetProcessNameAndID();
                CreateSelectProcessWindow();
            }

            else if(LOWORD(wParam) == ID_CHANGE_VALUE)
            {
                if((!SelectedAddressFrozen()) && (GetMatchCount(scanner))) CreateChangeValueWindow();
            }

            else if(LOWORD(wParam) == ID_NEW_SCAN)
            {
                if(scanner) ResetScan(scanner, false, true);
            }

        break;

        default:

            return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    return 0;
}

#endif

