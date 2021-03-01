#ifndef _MAIN_WINDOW_EVENT_HANDLERS_H
#define _MAIN_WINDOW_EVENT_HANDLERS_H

// Runs when left mouse button is clicked inside ListView on MainWindow if ScanRunning == false.

void ProcessListViewLeftClickEvent(void)
{
    int8 value[256];

    SelectedItem = (int32)SendMessageA(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if((SelectedItem != -1) && (StringLength(value)))
    {
        ListView_GetItemText(ListView, SelectedItem, 1, value, sizeof(value) - 1);

        if(SelectedAddressFrozen())
        {
            value[FindFirstOccurrenceOfString(value, " (FROZEN)", false)] = null;
        }

        SendMessageA(Value, WM_SETTEXT, 0, (LPARAM)value);
    }

    (SelectedAddressFrozen()) ? EnableWindow(ChangeValue, false) : EnableWindow(ChangeValue, (SelectedItem > -1));
}

// Runs when right mouse button is clicked inside ListView on MainWindow.

void ProcessListViewRightClickEvent(HWND hWnd)
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
                if(StringCompare(frozen_addresses[offset], buffer, false))
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

            TrackPopupMenu(PopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, x, y, 0, hWnd, 0);
        }
    }
}

// Runs when freeze/unfreeze option is toggled from ListView popup menu on MainWindow.

void ProcessFreezeValueButtonEvent(void)
{
    int8 address[256] = { 0 };
    int8 value[256] = { 0 };

    uint32 frozen_index;
    bool frozen;

    frozen = false;

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address) - 1);
    ListView_GetItemText(ListView, SelectedItem, 1, value, sizeof(value) - 1);

    for(frozen_index = 0; frozen_index < NumberOfAddressesFrozen; frozen_index++)
    {
        if(StringCompare(frozen_addresses[frozen_index], address, false))
        {
            frozen = true;
            break;
        }
    }

    if(!frozen)
    {
        if(NumberOfAddressesFrozen < FREEZE_LIMIT)
        {
            MemoryZero(&frozen_addresses[frozen_index], sizeof(frozen_addresses[frozen_index]));
            CopyMemory(&frozen_addresses[frozen_index], address, sizeof(frozen_addresses[frozen_index]));

            MemoryZero(&frozen_values[frozen_index], sizeof(frozen_values[frozen_index]));
            CopyMemory(&frozen_values[frozen_index], value, sizeof(frozen_values[frozen_index]));

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
    bool frozen;

    frozen = false;

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address) - 1);

    for(frozen_index = 0; frozen_index <= NumberOfAddressesFrozen; frozen_index++)
    {
        if(StringCompare(frozen_addresses[frozen_index], address, false))
        {
            frozen = true;
            break;
        }
    }

    if(frozen)
    {
        ListView_SetItemText(ListView, SelectedItem, 1, frozen_values[frozen_index]);

        MemoryZero(&frozen_addresses[frozen_index], sizeof(frozen_addresses[frozen_index]));
        MemoryZero(&frozen_values[frozen_index], sizeof(frozen_values[frozen_index]));

        NumberOfAddressesFrozen--;
    }

    EnableWindow(ChangeValue, true);
}

// Runs when "Select Process" button is clicked.

void ProcessSelectProcessButtonEvent(void)
{
    if(IndexOfSelectedProcess > -1)
    {
        HANDLE process;
        unsigned int process_id;
        bool error;
        char selected_process[256];
        char pid[256];

        CopyString(selected_process, pids[IndexOfSelectedProcess], sizeof(selected_process) - 1);

        if(StringLength(selected_process))
        {
            CopyString(pid, pids[IndexOfSelectedProcess], sizeof(pid) - 1);
            CopyString(selected_pid, pid, sizeof(selected_pid) - 1);
            SendMessage(Pid, WM_SETTEXT, 0, (LPARAM)processes[IndexOfSelectedProcess]);

            error = false;
        }

        else
        {
            ResetScan(scanner, true, true);
            error = true;
        }

        process_id = (unsigned int)StringToInteger(pid, FMT_INT_DECIMAL);

        process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, process_id);

        if(process)
        {
            if(scanner)
            {
                char data_size[256];

                LRESULT selection_id = SendMessage(DataSize, CB_GETCURSEL, 0, 0);

                if(selection_id > -1) CopyString(data_size, (char *)data_sizes[selection_id], sizeof(data_size) - 1);

                if(CurrentProcess != process_id)
                {
                    CurrentProcess = process_id;
                    ResetScan(scanner, false, true);
                    FreeMemoryScanner(scanner);
                    scanner = CreateMemoryScanner(CurrentProcess, (unsigned short)StringToInteger(data_size, FMT_INT_DECIMAL));
                }
            }

            CloseHandle(process);
            EnableWindow(Scan, true);

            error = false;
        }

        else
        {
            if(scanner) 
            {
                ResetScan(scanner, true, true);
            }

            EnableWindow(Scan, false);
            EnableWindow(ChoosePid, true);

            error = true;
        }

        DestroyWindow(PidDlg);
        EnableWindow(MainWindow, true);
        SetForegroundWindow(MainWindow);
    }
}


void ProcessMainWindowCloseEvent(HWND hWnd)
{
    FreeMemoryScanner(scanner);

    TerminateThread(FreezeThread, 0);
    WaitForSingleObject(FreezeThread, INFINITE);
    CloseHandle(FreezeThread);

    TerminateThread(MonitorSelectedProcessThread, 0);
    WaitForSingleObject(MonitorSelectedProcessThread, INFINITE);
    CloseHandle(MonitorSelectedProcessThread);

    TerminateThread(ScanThread, 0);
    WaitForSingleObject(ScanThread, INFINITE);
    CloseHandle(ScanThread);

    DestroyWindow(hWnd);
}

#endif
