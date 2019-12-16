#ifndef _WINDOW_PROCS_H
#define _WINDOW_PROCS_H

/* ************************************************************************************ */


LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SelectPidDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChangeValueDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


// The window procedure for the main window.

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
        case WM_CREATE:

           CreateMainDialogUI(hWnd);
           CenterWindow(hWnd);

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

        case WM_CLOSE:

           CleanupAndTerminateApplication(hWnd);

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

            else if(LOWORD(wParam) == ID_FILE_EXIT)
            {
                PostQuitMessage(0);
            }

            else if(LOWORD(wParam) == ID_HELP_ABOUT)
            {
                CreateAboutDialog(hWnd);
            }

            else if(LOWORD(wParam) == ID_SCAN)
            {
                TerminateThread(ScanThread, 0);
                CloseHandle(ScanThread);
                ScanThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ProcessScan, 0, 0, 0);
            }

            else if(LOWORD(wParam) == ID_SELECT_PROCESS)
            {
                GetProcessNameAndID();
                CreateChooseProcessDialogUI();
            }

            else if(LOWORD(wParam) == ID_CHANGE_VALUE)
            {
                if((!SelectedAddressFrozen()) && (GetMatchCount(scanner))) CreateChangeValueDialogUI();
            }

            else if(LOWORD(wParam) == ID_NEW_SCAN)
            {
                if(scanner) ResetScan(scanner, FALSE, TRUE);
            }

        break;

        case WM_DESTROY:

            PostQuitMessage(0);

        break;

        default:
            return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    return 0;
}

/* ************************************************************************************ */


/* ************************************************************************************ */

// The window procedure for the process selection dialog.


LRESULT CALLBACK SelectPidDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
        case WM_CLOSE:

            DestroyWindow(hWnd);
            EnableWindow(MainWindow, TRUE);
            SetForegroundWindow(MainWindow);

        break;

        case WM_CREATE:

            CenterWindow(hWnd, MainWindow); 

        break;

        case WM_COMMAND:

            if(LOWORD(wParam) == ID_CHOOSE_PROCESS)
            {
                ProcessChooseProcessButtonEvent();
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

/* ************************************************************************************ */


/* ************************************************************************************ */


// The window procedure for the dialog that modifies the value of a scan result.


LRESULT CALLBACK ChangeValueDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
        case WM_DESTROY:

            EnableWindow(MainWindow, TRUE);
            SetForegroundWindow(MainWindow);

        break;

        case WM_CREATE:

            CreateChangeValueDialogUIChildren(hWnd);
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

/* ************************************************************************************ */

#endif
