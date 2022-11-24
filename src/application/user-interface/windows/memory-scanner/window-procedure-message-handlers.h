#ifndef _MEMORY_SCANNER_WINDOW_PROCEDURE_MESSAGE_HANDLERS_H
#define _MEMORY_SCANNER_WINDOW_PROCEDURE_MESSAGE_HANDLERS_H

/* Runs when left mouse button is clicked inside ListView on MemoryScannerWindow if ScanRunning == false. */

VOID HandleListViewLeftClickEvent(VOID)
{
    SelectedItem = (int32)SendMessageA(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
    ListView_GetItemText(ListView, SelectedItem, 0, SelectedItemAddress, sizeof(SelectedItemAddress));
    ListView_GetItemText(ListView, SelectedItem, 1, SelectedItemValue, sizeof(SelectedItemValue)); 

    if(SelectedItem != -1)
    {
        if(IsAddressFrozen(SelectedItemAddress))
        {
            int32 index = FindFirstOccurrenceOfString(SelectedItemValue, (string)" (FROZEN)", false);

            if(index != -1)
            {
                SelectedItemValue[index] = null;
            }
        }

        SendMessageA(Value, WM_SETTEXT, 0, (LPARAM)SelectedItemValue);
    }

    (IsAddressFrozen(SelectedItemAddress)) ? EnableWindow(ChangeValue, false) : EnableWindow(ChangeValue, (SelectedItem > -1));
}

/* Runs when right mouse button is clicked inside ListView on MemoryScannerWindow. */

VOID HandleListViewRightClickEvent(HWND window)
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
                if(NumberOfAddressesFrozen < FREEZE_LIMIT)
                {
                    EnableWindow(ChangeValue, true);
                    InsertMenu(popup_menu, 0, MF_STRING, ID_FREEZE_VALUE, (string)"Freeze Value");
                }
            }

            TrackPopupMenu(popup_menu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, x, y, 0, window, 0);
        }
    }
}

/* Runs when freeze/thaw option is toggled from ListView popup menu on MemoryScannerWindow. */

VOID HandleFreezeValueButtonEvent(VOID)
{
    SelectedItem = (int32)SendMessageA(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if(SelectedItem != -1)
    {
        if(!IsAddressFrozen(SelectedItemAddress))
        {
            if(SuspendThread(FreezeThread) != (DWORD)-1)
            {
                if(NumberOfAddressesFrozen < FREEZE_LIMIT)
                {
                    CopyString(FrozenAddresses[NumberOfAddressesFrozen], SelectedItemAddress, sizeof(FrozenAddresses[NumberOfAddressesFrozen]) - 1);
                    CopyString(FrozenValues[NumberOfAddressesFrozen], SelectedItemValue, sizeof(FrozenValues[NumberOfAddressesFrozen]) - 1);

                    if((StringCompare(FrozenAddresses[NumberOfAddressesFrozen], SelectedItemAddress, false)) && (StringCompare(FrozenValues[NumberOfAddressesFrozen], SelectedItemValue, false)))
                    {
                        ListView_SetItemText(ListView, SelectedItem, 1, StringConcat(SelectedItemValue, (string)" (FROZEN)"));
                        NumberOfAddressesFrozen++;
                    }
                }

                ResumeThread(FreezeThread);
            }

            EnableWindow(ChangeValue, false);
        }
    }
}

VOID HandleThawValueButtonEvent(VOID)
{
    SelectedItem = (int32)SendMessageA(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if(SelectedItem != -1)
    {
        if(IsAddressFrozen(SelectedItemAddress))
        {
            if(SuspendThread(FreezeThread) != (DWORD)-1)
            {
                uint16 frozen_index;
                boolean address_frozen = false;

                for(frozen_index = 0; frozen_index < FREEZE_LIMIT; frozen_index++)
                {
                    if(StringCompare(FrozenAddresses[frozen_index], SelectedItemAddress, false)) 
                    {
                        address_frozen = true;
                        break;
                    }
                }

                if(address_frozen)
                {
                    ListView_SetItemText(ListView, SelectedItem, 1, FrozenValues[frozen_index]);

                    MemoryZero(FrozenAddresses[frozen_index], sizeof(FrozenAddresses[frozen_index]));
                    MemoryZero(FrozenValues[frozen_index], sizeof(FrozenValues[frozen_index]));

                    NumberOfAddressesFrozen--;
                }

                ResumeThread(FreezeThread);
            }

            EnableWindow(ChangeValue, true);
        }
    }
}

VOID HandleMainWindowCloseEvent(HWND window)
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
