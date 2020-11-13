#ifndef _MAIN_WINDOW_EVENT_HANDLERS_H
#define _MAIN_WINDOW_EVENT_HANDLERS_H

// Runs when left mouse button is clicked inside ListView on MainWindow if ScanRunning == false.

void ProcessListViewLeftClickEvent(void)
{
    int8 buffer[256];

    SelectedItem = (int32)SendMessage(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if((SelectedItem != -1) && (StringLength(buffer)))
    {
        ListView_GetItemText(ListView, SelectedItem, 1, buffer, sizeof(buffer));
        SendMessage(Value, WM_SETTEXT, 0, (LPARAM)buffer);
    }

    (SelectedAddressFrozen()) ? EnableWindow(ChangeValue, false) : EnableWindow(ChangeValue, true);
}

// Runs when right mouse button is clicked inside ListView on MainWindow if ScanRunning == false.

void ProcessListViewRightClickEvent(HWND hWnd)
{
    SelectedItem = (int32)SendMessage(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if(SelectedItem != -1)
    {
        POINT pos;

        if(GetCursorPos(&pos))
        {
            int8 buffer[256];

            ListView_GetItemText(ListView, SelectedItem, 0, buffer, sizeof(buffer));

            int32 x = pos.x;
            int32 y = pos.y;

            uint32 offset;
            bool address_frozen = false;

            for(offset = 0; offset < NumberOfAddressesFrozen; offset++)
            {
                if(StringCompare(frozen_addresses[offset], buffer, false))
                {
                    address_frozen = true;
                    break;
                }
            }

            HMENU PopupMenu = CreatePopupMenu();

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

            TrackPopupMenu(PopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, x, y, 0, hWnd, 0);
        }
    }
}

// Runs when freeze/unfreeze option is toggled from ListView popup menu on MainWindow.

void ProcessFreezeValueButtonEvent(void)
{
    int8 address[256], value[512];

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address));
    ListView_GetItemText(ListView, SelectedItem, 1, value, sizeof(value));

    uint32 offset;
    bool frozen = false;

    for(offset = 0; offset < NumberOfAddressesFrozen; offset++)
    {
        if(StringCompare(frozen_addresses[offset], address, false))
        {
            frozen = true;
            break;
        }
    }

    if(!frozen)
    {
        if(NumberOfAddressesFrozen < FREEZE_LIMIT)
        {
            MemoryZero(&frozen_addresses[NumberOfAddressesFrozen], sizeof(frozen_addresses[NumberOfAddressesFrozen]));
            CopyMemory(&frozen_addresses[NumberOfAddressesFrozen], address, sizeof(frozen_addresses[NumberOfAddressesFrozen]));

            MemoryZero(&frozen_values[NumberOfAddressesFrozen], sizeof(frozen_values[NumberOfAddressesFrozen]));
            CopyMemory(&frozen_values[NumberOfAddressesFrozen], value, sizeof(frozen_values[NumberOfAddressesFrozen]));

            ListView_SetItemText(ListView, SelectedItem, 1, StringConcat(value, (cstring)" (FROZEN)"));

            NumberOfAddressesFrozen++;
        }
    }

    EnableWindow(ChangeValue, false);
}

void ProcessUnfreezeValueButtonEvent(void)
{
    int8 address[256];
    uint32 offset;

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address));

    for(offset = 0; offset < NumberOfAddressesFrozen; offset++)
    {
        if(StringCompare(frozen_addresses[offset], address, false))
        {
            ListView_SetItemText(ListView, SelectedItem, 1, frozen_values[offset]);

            MemoryZero(&frozen_addresses[offset], sizeof(frozen_addresses[offset]));
            MemoryZero(&frozen_values[offset], sizeof(frozen_values[offset]));

            NumberOfAddressesFrozen--;
        }
    }

    EnableWindow(ChangeValue, true);
}

// Runs when "Select Process" button is clicked.

void ProcessSelectProcessButtonEvent(void)
{
    if(IndexOfSelectedProcess > -1)
    {
        bool error;
        int8 selected_process[256];
        int8 pid[256];

        CopyString(selected_process, pids[IndexOfSelectedProcess], sizeof(selected_process));

        if(StringLength(selected_process))
        {
            CopyString(pid, pids[IndexOfSelectedProcess], sizeof(pid));
            CopyString(PID, pid, sizeof(PID));
            SendMessage(Pid, WM_SETTEXT, 0, (LPARAM)processes[IndexOfSelectedProcess]);

            error = false;
        }

        else
        {
            ResetScan(scanner, true, true);
            error = true;
        }

        uint32 process_id = (uint32)StringToInteger(pid, FMT_INT_DECIMAL);

        HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, process_id);

        if(process)
        {
            if(scanner)
            {
                int8 data_size[256];
                LRESULT selection_id = SendMessage(DataSize, CB_GETCURSEL, 0, 0);

                if(selection_id > -1) CopyString(data_size, (cstring)data_sizes[selection_id], sizeof(data_size));

                if(CurrentProcess != process_id)
                {
                    CurrentProcess = process_id;
                    ResetScan(scanner, false, true);
                    FreeMemoryScanner(scanner);
                    scanner = CreateMemoryScanner(CurrentProcess, (uint16)StringToInteger(data_size, FMT_INT_DECIMAL));
                }
            }

            CloseHandle(process);
        }

        else
        {
            error = true;
            EnableWindow(ChoosePid, true);
            if(scanner) ResetScan(scanner, true, true);
        }

        DestroyWindow(PidDlg);
        EnableWindow(MainWindow, true);
        (error) ? EnableWindow(Scan, false) : EnableWindow(Scan, true);
        SetForegroundWindow(MainWindow);
    }
}

void ProcessMainWindowCloseEvent(HWND hWnd)
{
    TerminateThread(FreezeThread, 0);
    WaitForSingleObject(FreezeThread, INFINITE);
    CloseHandle(FreezeThread);
    TerminateThread(MonitorSelectedProcessThread, 0);
    WaitForSingleObject(MonitorSelectedProcessThread, INFINITE);
    CloseHandle(MonitorSelectedProcessThread);
    TerminateThread(ScanThread, 0);
    WaitForSingleObject(ScanThread, INFINITE);
    CloseHandle(ScanThread);

    if(scanner)
    {
        FreeMemoryScanner(scanner);
    }

    DestroyWindow(hWnd);
}

#endif
