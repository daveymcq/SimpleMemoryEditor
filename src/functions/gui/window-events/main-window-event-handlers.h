#ifndef _MAIN_WINDOW_EVENT_HANDLERS_H
#define _MAIN_WINDOW_EVENT_HANDLERS_H

/* Runs when left mouse button is clicked inside ListView on MainWindow if ScanRunning == false. */

void ProcessListViewLeftClickEvent(void)
{
    int8 value[256];

    SelectedItem = (int32)SendMessageA(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if((SelectedItem != -1) && (StringLength(value)))
    {
        ListView_GetItemText(ListView, SelectedItem, 1, value, sizeof(value) - 1);

        if(SelectedAddressFrozen())
        {
            value[FindFirstOccurrenceOfString(value, (string)" (FROZEN)", false)] = null;
        }

        SendMessageA(Value, WM_SETTEXT, 0, (LPARAM)value);
    }

    (SelectedAddressFrozen()) ? EnableWindow(ChangeValue, false) : EnableWindow(ChangeValue, (SelectedItem > -1));
}

/* Runs when right mouse button is clicked inside ListView on MainWindow. */

void ProcessListViewRightClickEvent(HWND window)
{
    SelectedItem = (int32)SendMessageA(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if(SelectedItem != -1)
    {
        POINT pos;

        if(GetCursorPos(&pos))
        {
            int8 buffer[256];

            DWORD x, y;
            uint32 offset;
            bool address_frozen;
            HMENU PopupMenu;

            x = pos.x;
            y = pos.y;

            address_frozen = false;
            PopupMenu = CreatePopupMenu();

            ListView_GetItemText(ListView, SelectedItem, 0, buffer, sizeof(buffer) - 1);

            for(offset = 0; offset < NumberOfAddressesFrozen; offset++)
            {
                if(StringCompare(FrozenAddresses[offset], buffer, false))
                {
                    address_frozen = true;
                    break;
                }
            }

            if(address_frozen)
            {
                EnableWindow(ChangeValue, false);
                InsertMenu(PopupMenu, 0, MF_STRING, ID_UNFREEZE_VALUE, "Unfreeze Value");
            }

            else
            {
                EnableWindow(ChangeValue, true);
                InsertMenu(PopupMenu, 0, MF_STRING, ID_FREEZE_VALUE, "Freeze Value");
            }

            TrackPopupMenu(PopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, x, y, 0, window, 0);
        }
    }
}

/* Runs when freeze/unfreeze option is toggled from ListView popup menu on MainWindow. */

void ProcessFreezeValueButtonEvent(void)
{
    int8 address[256] = { 0 };
    int8 value[256] = { 0 };

    uint32 frozen_index;
    bool frozen = false;

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address) - 1);
    ListView_GetItemText(ListView, SelectedItem, 1, value, sizeof(value) - 1);

    for(frozen_index = 0; frozen_index < NumberOfAddressesFrozen; frozen_index++)
    {
        if(StringCompare(FrozenAddresses[frozen_index], address, false))
        {
            frozen = true;
            break;
        }
    }

    if(!frozen)
    {
        if(NumberOfAddressesFrozen < FREEZE_LIMIT)
        {
            MemoryZero(&FrozenAddresses[frozen_index], sizeof(FrozenAddresses[frozen_index]));
            CopyMemory(&FrozenAddresses[frozen_index], address, sizeof(FrozenAddresses[frozen_index]));

            MemoryZero(&FrozenValues[frozen_index], sizeof(FrozenValues[frozen_index]));
            CopyMemory(&FrozenValues[frozen_index], value, sizeof(FrozenValues[frozen_index]));

            ListView_SetItemText(ListView, SelectedItem, 1, StringConcat(value, " (FROZEN)"));

            NumberOfAddressesFrozen++;
        }
    }

    EnableWindow(ChangeValue, false);
}

void ProcessUnfreezeValueButtonEvent(void)
{
    int8 address[256] = { 0 };

    uint32 frozen_index;
    bool frozen = false;

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address) - 1);

    for(frozen_index = 0; frozen_index <= NumberOfAddressesFrozen; frozen_index++)
    {
        if(StringCompare(FrozenAddresses[frozen_index], address, false))
        {
            frozen = true;
            break;
        }
    }

    if(frozen)
    {
        ListView_SetItemText(ListView, SelectedItem, 1, FrozenValues[frozen_index]);

        MemoryZero(&FrozenAddresses[frozen_index], sizeof(FrozenAddresses[frozen_index]));
        MemoryZero(&FrozenValues[frozen_index], sizeof(FrozenValues[frozen_index]));

        NumberOfAddressesFrozen--;
    }

    EnableWindow(ChangeValue, true);
}

/* Runs when "Select Process" button is clicked. */

void ProcessSelectProcessButtonEvent(void)
{
    if(IndexOfSelectedProcess > -1)
    {
        int8 selected_process[256];
        int8 pid[256];

        HANDLE process;
        uint32 process_id;
        bool error;

        CopyString(selected_process, Pids[IndexOfSelectedProcess], sizeof(selected_process) - 1);

        if(StringLength(selected_process))
        {
            CopyString(pid, Pids[IndexOfSelectedProcess], sizeof(pid) - 1);
            CopyString(SelectedPid, pid, sizeof(SelectedPid) - 1);
            SendMessage(Pid, WM_SETTEXT, 0, (LPARAM)Processes[IndexOfSelectedProcess]);

            error = false;
        }

        else
        {
            ResetScan(Scanner, true, true);
            error = true;
        }

        process_id = (uint32)StringToInteger(pid, FMT_INT_DECIMAL);

        process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, process_id);

        if(process)
        {
            if(Scanner)
            {
                int8 data_size[256];

                LRESULT selection_id = SendMessage(DataSize, CB_GETCURSEL, 0, 0);

                if(selection_id > -1) 
                {
                    CopyString(data_size, (string)DataSizes[selection_id], sizeof(data_size) - 1);
                }

                if(CurrentProcess != process_id)
                {
                    CurrentProcess = process_id;
                    ResetScan(Scanner, false, true);
                    FreeMemoryScanner(Scanner);
                    Scanner = CreateMemoryScanner(CurrentProcess, (uint16)StringToInteger(data_size, FMT_INT_DECIMAL));
                }
            }

            CloseHandle(process);
            EnableWindow(Scan, true);

            error = false;
        }

        else
        {
            if(Scanner) 
            {
                ResetScan(Scanner, true, true);
            }

            EnableWindow(Scan, false);
            EnableWindow(ChoosePid, true);

            error = true;
        }

        DestroyWindow(SelectPidWindow);
        EnableWindow(MainWindow, true);
        SetForegroundWindow(MainWindow);
    }
}

void ProcessMainWindowCloseEvent(HWND window)
{
    FreeMemoryScanner(Scanner);

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
