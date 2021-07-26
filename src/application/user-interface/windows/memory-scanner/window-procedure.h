#ifndef _MEMORY_SCANNER_WINDOW_PROCEDURE_H
#define _MEMORY_SCANNER_WINDOW_PROCEDURE_H

/* ************************************************************************************ */

/* ************************************************************************************ */

/* The window procedure for the memory scanner window. */

LRESULT CALLBACK MainWindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch(message)
    {
        case WM_CREATE:

            CenterWindow(window);

        break;

        case WM_CLOSE:

            HandleMainWindowCloseEvent(window);

        break;

        case WM_DESTROY:

            PostQuitMessage(0);

        break;

        case WM_COMMAND:

            if(LOWORD(wparam) == ID_FREEZE_VALUE)
            {
                HandleFreezeValueButtonEvent();
            }

            else if(LOWORD(wparam) == ID_THAW_VALUE)
            {
                HandleThawValueButtonEvent();
            }

            else if(LOWORD(wparam) == ID_SCAN)
            {
                if(ScanThread)
                {
                    WaitForSingleObject(ScanThread, INFINITE);

                    if(!ScanRunning)
                    {
                        CloseHandle(ScanThread);
                    }
                }

                ScanThread = CreateThread(null, null, (LPTHREAD_START_ROUTINE)ProcessScan, null, null, null);
            }

            else if(LOWORD(wparam) == ID_SELECT_PROCESS)
            {
                GetProcessNameAndID();
                CreateSelectProcessWindow();
            }

            else if(LOWORD(wparam) == ID_CHANGE_VALUE)
            {
                if((AddressFrozen(SelectedItemAddress) == -1) && (GetMatchCount(Scanner) > 0))
                {
                    CreateChangeValueWindow();
                }
            }

            else if(LOWORD(wparam) == ID_NEW_SCAN)
            {
                if(Scanner) 
                {
                    ResetScan(Scanner, false, false);
                }
            }

        break;

        case WM_NOTIFY:

            if(!ScanRunning)
            {
                if(((LPNMHDR)lparam)->code == NM_CLICK)
                {
                    HandleListViewLeftClickEvent();
                }

                else if((((LPNMHDR)lparam)->code == NM_RCLICK) && (((LPNMHDR)lparam)->idFrom == ID_LISTVIEW))
                {
                    HandleListViewRightClickEvent(window);
                }
            }

        break;

        default:

            return DefWindowProc(window, message, wparam, lparam);
    }

    return 0;
}

#endif

