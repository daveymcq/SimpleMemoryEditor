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

        if(AddressFrozen(SelectedItemAddress) != -1)
        {
            SelectedItemValue[FindFirstOccurrenceOfString(SelectedItemValue, (string)" (FROZEN)", false)] = null;
        }

        SendMessageA(Value, WM_SETTEXT, 0, (LPARAM)SelectedItemValue);
    }

    (AddressFrozen(SelectedItemAddress)) ? EnableWindow(ChangeValue, false) : EnableWindow(ChangeValue, (SelectedItem > -1));
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
            uint32 offset;
            bool address_frozen;
            HMENU popup_menu;

            x = pos.x;
            y = pos.y;

            ListView_GetItemText(ListView, SelectedItem, 0, SelectedItemAddress, sizeof(SelectedItemAddress));
            ListView_GetItemText(ListView, SelectedItem, 1, SelectedItemValue, sizeof(SelectedItemValue)); 

            address_frozen = (AddressFrozen(SelectedItemAddress) != -1);
            popup_menu = CreatePopupMenu();

            if(address_frozen)
            {
                EnableWindow(ChangeValue, false);
                InsertMenu(popup_menu, 0, MF_STRING, ID_THAW_VALUE, "Thaw Value");
            }

            else
            {
                if(NumberOfAddressesFrozen < FREEZE_LIMIT)
                {
                    EnableWindow(ChangeValue, true);
                    InsertMenu(popup_menu, 0, MF_STRING, ID_FREEZE_VALUE, "Freeze Value");
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
        int32 frozen_index = AddressFrozen(SelectedItemAddress);

        if(frozen_index == -1)
        {
            frozen_index = NumberOfAddressesFrozen;

            if(NumberOfAddressesFrozen < FREEZE_LIMIT)
            {
                if(SuspendThread(FreezeThread) != -1)
                {
                    CopyString(FrozenAddresses[frozen_index], SelectedItemAddress, sizeof(FrozenAddresses[frozen_index]) - 1);
                    CopyString(FrozenValues[frozen_index], SelectedItemValue, sizeof(FrozenValues[frozen_index]) - 1);

                    ListView_SetItemText(ListView, SelectedItem, 1, StringConcat(SelectedItemValue, " (FROZEN)"));

                    NumberOfAddressesFrozen++;

                    ResumeThread(FreezeThread);
                }
            }
        }

        EnableWindow(ChangeValue, false);
    }
}

void HandleThawValueButtonEvent(void)
{
    SelectedItem = (int32)SendMessageA(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if(SelectedItem != -1)
    {
        int32 frozen_index = AddressFrozen(SelectedItemAddress);

        if(frozen_index != -1)
        {
            if(SuspendThread(FreezeThread) != -1)
            {
                ListView_SetItemText(ListView, SelectedItem, 1, FrozenValues[frozen_index]);

                MemoryZero(&FrozenAddresses[frozen_index], sizeof(FrozenAddresses[frozen_index]));
                MemoryZero(&FrozenValues[frozen_index], sizeof(FrozenValues[frozen_index]));

                NumberOfAddressesFrozen--;

                ResumeThread(FreezeThread);
            }
        }

        EnableWindow(ChangeValue, true);
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
