#ifndef _MEMORY_SCANNER_WINDOW_PROCEDURE_H
#define _MEMORY_SCANNER_WINDOW_PROCEDURE_H

/* ************************************************************************************ */

/* ************************************************************************************ */

/* The window procedure for the memory scanner window. */

LRESULT CALLBACK MainWindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    LVCOLUMNA address_column;
    LVCOLUMNA value_column;
    
    switch(message)
    {
        case WM_CREATE:
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
                if((!IsAddressFrozen(SelectedItemAddress)) && (GetMatchCount(Scanner)))
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

        case WM_DPICHANGED:

            UpdateLayoutForDpi(MemoryScannerWindow, CW_USEDEFAULT, CW_USEDEFAULT, 625, 405);
            UpdateLayoutForDpi(ListView, 10, 10, 598, 225);
            UpdateLayoutForDpi(SearchCondition, 150, 245, 125, 200);
            UpdateLayoutForDpi(SearchConditionLabel, 10, 245, 100, 25);
            UpdateLayoutForDpi(ValueLabel, 310, 250, 100, 25);
            UpdateLayoutForDpi(Value, 400, 247, 100, 20);
            UpdateLayoutForDpi(ChangeValue, 510, 245, 100, 25);
            UpdateLayoutForDpi(NewScan, 510, 275, 100, 25);
            UpdateLayoutForDpi(Pid, 10, 280, 150, 25);
            UpdateLayoutForDpi(ChoosePid, 175, 275, 100, 25);
            UpdateLayoutForDpi(DataSizeLabel, 310, 280, 100, 25);
            UpdateLayoutForDpi(DataSize, 400, 275, 100, 200);
            UpdateLayoutForDpi(Scan, 10, 315, 600, 50);

            address_column.cx = MulDiv(298, ScreenDPI, 96); 
            value_column.cx = MulDiv(298, ScreenDPI, 96);

            address_column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
            address_column.pszText = (string)"Address";
            address_column.iSubItem = 0;

            value_column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
            value_column.pszText = (string)"Value";
            value_column.iSubItem = 1;

            SendMessageA(ListView, LVM_INSERTCOLUMN, 0, (LPARAM)&address_column);
            SendMessageA(ListView, LVM_INSERTCOLUMN, 1, (LPARAM)&value_column);
            SendMessageA(SearchCondition, CB_ADDSTRING, 0, (LPARAM)SearchConditions[SEARCH_EQUALS]);
            SendMessageA(ListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
            
            CenterWindow(MemoryScannerWindow);

        break;

        default:

            return DefWindowProc(window, message, wparam, lparam);
    }

    return 0;
}

#endif

