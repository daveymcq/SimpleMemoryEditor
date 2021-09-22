#ifndef _MEMORY_SCANNER_WINDOW_PROCEDURE_MESSAGE_HANDLERS_H
#define _MEMORY_SCANNER_WINDOW_PROCEDURE_MESSAGE_HANDLERS_H

/* Runs when left mouse button is clicked inside ListView on MemoryScannerWindow if ScanRunning == false. */

void HandleListViewLeftClickEvent(void)
{
    SelectedItem = (int32)SendMessageA(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if(SelectedItem != -1)
    {
        ListView_GetItemText(ListView, SelectedItem, 0, SelectedItemAddress, sizeof(SelectedItemAddress));
        ListView_GetItemText(ListView, SelectedItem, 1, SelectedItemValue, sizeof(SelectedItemValue)); 

        if(IsAddressFrozen(SelectedItemAddress))
        {
            SelectedItemValue[FindFirstOccurrenceOfString(SelectedItemValue, (string)" (FROZEN)", false)] = null;
        }

        SendMessageA(Value, WM_SETTEXT, 0, (LPARAM)SelectedItemValue);
    }

    (IsAddressFrozen(SelectedItemAddress)) ? EnableWindow(ChangeValue, false) : EnableWindow(ChangeValue, (SelectedItem > -1));
}

/* Runs when right mouse button is clicked inside ListView on MemoryScannerWindow. */

void HandleListViewRightClickEvent(HWND window)
{
    SelectedItem = (int32)SendMessageA(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if(SelectedItem != -1)
    {
        POINT pos;

        if(GetCursorPos(&pos))
        {
            DWORD x, y;
            HMENU popup_menu;

            x = pos.x;
            y = pos.y;

            ListView_GetItemText(ListView, SelectedItem, 0, SelectedItemAddress, sizeof(SelectedItemAddress));
            ListView_GetItemText(ListView, SelectedItem, 1, SelectedItemValue, sizeof(SelectedItemValue)); 

            popup_menu = CreatePopupMenu();

            if(IsAddressFrozen(SelectedItemAddress))
            {
                EnableWindow(ChangeValue, false);
                InsertMenu(popup_menu, 0, MF_STRING, ID_THAW_VALUE, (string)"Thaw Value");
            }

            else
            {
                if(!AddressFrozen)
                {
                    EnableWindow(ChangeValue, true);
                    InsertMenu(popup_menu, 0, MF_STRING, ID_FREEZE_VALUE, (string)"Freeze Value");
                }
            }

            TrackPopupMenu(popup_menu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, x, y, 0, window, 0);
        }
    }
}

/* Runs when freeze/unfreeze option is toggled from ListView popup menu on MemoryScannerWindow. */

void HandleFreezeValueButtonEvent(void)
{
    SelectedItem = (int32)SendMessageA(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if(SelectedItem != -1)
    {
        if(!IsAddressFrozen(SelectedItemAddress))
        {
            if(SuspendThread(FreezeThread) != -1)
            {
                CopyString(FrozenAddress, SelectedItemAddress, sizeof(FrozenAddress) - 1);
                CopyString(FrozenValue, SelectedItemValue, sizeof(FrozenValue) - 1);

                ListView_SetItemText(ListView, SelectedItem, 1, StringConcat(SelectedItemValue, (string)" (FROZEN)"));

                AddressFrozen = true;

                ResumeThread(FreezeThread);
            }

            EnableWindow(ChangeValue, false);
        }
    }
}

void HandleThawValueButtonEvent(void)
{
    SelectedItem = (int32)SendMessageA(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if(SelectedItem != -1)
    {
        if(IsAddressFrozen(SelectedItemAddress))
        {
            if(SuspendThread(FreezeThread) != -1)
            {
                ListView_SetItemText(ListView, SelectedItem, 1, FrozenValue);

                MemoryZero(FrozenAddress, sizeof(FrozenAddress));
                MemoryZero(FrozenValue, sizeof(FrozenValue));

                AddressFrozen = false;

                ResumeThread(FreezeThread);
            }

            EnableWindow(ChangeValue, true);
        }
    }
}

void HandleMainWindowCloseEvent(HWND window)
{
    if(Scanner)
    {
        FreeMemoryScanner(Scanner);
    }

    if(FreezeThread && TerminateThread(FreezeThread, 0))
    {
        WaitForSingleObject(FreezeThread, INFINITE);
        CloseHandle(FreezeThread);
    }

    if(MonitorSelectedProcessThread && TerminateThread(MonitorSelectedProcessThread, 0))
    {
        WaitForSingleObject(MonitorSelectedProcessThread, INFINITE);
        CloseHandle(MonitorSelectedProcessThread);
    }

    if(ScanThread && TerminateThread(ScanThread, 0))
    {
        WaitForSingleObject(ScanThread, INFINITE);
        CloseHandle(ScanThread);
    }

    DestroyWindow(window);
}

#endif
