#ifndef _MEMORY_SCANNER_WINDOW_FUNCTIONS_H
#define _MEMORY_SCANNER_WINDOW_FUNCTIONS_H

/* Add scan results to ListView control. */

void AddItemToListView(string address, string value)
{
    static LVITEMA item;

    item.mask        = LVIF_TEXT;
    item.iSubItem    = 0;
    item.pszText     = (string)address;

    SendMessageA(ListView, LVM_INSERTITEM, 0, (LPARAM)&item);

    item.iSubItem    = 1;
    item.pszText     = (string)value;

    SendMessageA(ListView, LVM_SETITEM, 0, (LPARAM)&item);
}

/* Resets all previosly filtered addresses. */

void ResetScan(MEMORY_BLOCK *mblock, bool reset_pid, bool disable_process_monitor)
{
    uint16 index;
    MEMORY_BLOCK *mb;

    SelectedProcessOpen = false;
    NumberOfAddressesFrozen = 0;
    FirstScanNotRun = true;
    IsAddressFrozen = false;
    SelectedItem = -1;
    mb = mblock;

    MemoryZero(FrozenAddress, sizeof(FrozenAddress));
    MemoryZero(FrozenValue, sizeof(FrozenValue));

    EnableWindow(Scan, !reset_pid);
    EnableWindow(ChangeValue, false);
    EnableWindow(NewScan, false);

    SetDlgItemText(MemoryScannerWindow, ID_VALUE, 0);

    SendMessageA(DataSize, CB_RESETCONTENT, 0, 0);
    SendMessageA(SearchCondition, CB_RESETCONTENT, 0, 0);
    SendMessageA(SearchCondition, CB_ADDSTRING, 0, (LPARAM)SearchConditions[SEARCH_EQUALS]);

    ListView_DeleteAllItems(ListView);

    while(mb)
    {
        MemorySet(mb->match_flag, 0xff, mb->size / 8); 
        mb->matches = mb->size;
        mb = mb->next;
    }

    if(reset_pid)
    {
        string msg = "*No Process Selected*";
        SendMessageA(Pid, WM_SETTEXT, 0, (LPARAM)msg);

        EnableWindow(ChoosePid, true);
        EnableWindow(DataSize, false);
        EnableWindow(SearchCondition, false);
        EnableWindow(Value, false);
    }

    for(index = 0; index < ARRAYSIZE(Datatypes); index++)
    {
        SendMessageA(DataSize, CB_ADDSTRING, 0, (LPARAM)Datatypes[index]);
    }

    if(disable_process_monitor)
    {
        if(MonitorSelectedProcessThread && TerminateThread(MonitorSelectedProcessThread, 0))
        {
            WaitForSingleObject(MonitorSelectedProcessThread, INFINITE);
            CloseHandle(MonitorSelectedProcessThread);
        }
    }

    if(FreezeThread && TerminateThread(FreezeThread, 0))
    {
        WaitForSingleObject(FreezeThread, INFINITE);
        CloseHandle(FreezeThread);
    }
}

/* Calls ResetScan() if the selected thread terminates. */

DWORD WINAPI MonitorSelectedProcess(void)
{
    while(true)
    {
        DWORD code;
        GetExitCodeProcess(Scanner->process, &code);
        SelectedProcessOpen = (code == STILL_ACTIVE);

        if(!SelectedProcessOpen)
        {
            ResetScan(Scanner, true, true);
            break;
        }
    }

    return EXIT_SUCCESS;
}

/* Add scan results to memory scanner window. */

void DisplayScanResults(MEMORY_BLOCK *mblock, INTFMT display_format, uint32 display_limit)
{
    MEMORY_BLOCK *mb = mblock;
    uint32 limit = (GetMatchCount(mb) > display_limit) ? display_limit : (uint32)GetMatchCount(mb);

    while(mb)
    {
        uint64 offset;

        for(offset = 0; offset < mb->size; offset += mb->data_size)
        {
            if(AddressNotDiscarded(mb, offset))
            {
                int8 address[256];
                int8 val[256];

                IntegerToString((uint64)(uintptr_t)mb->address + offset, address, sizeof(address) - 1, FMT_INT_HEXADECIMAL);

                if(Type == TYPE_INTEGER)
                {
                    int64 value = PeekInteger(mb->process, mb->address + offset, mb->data_size);

                    if(display_format == FMT_INT_DECIMAL)
                    {
                        IntegerToString(value, val, sizeof(val) - 1, display_format);
                    }

                    else if(display_format == FMT_INT_HEXADECIMAL)
                    {
                        val[0] = '0';
                        val[1] = 'x';

                        IntegerToString(value, val + 2, sizeof(val) - 3, display_format);
                    }
                }

                else if(Type == TYPE_FLOAT)
                {
                    float value = PeekFloat(mb->process, mb->address + offset, mb->data_size);
                    DoubleToString((double)value, val, sizeof(val) - 1);
                }

                else if(Type == TYPE_DOUBLE)
                {
                    double value = PeekDouble(mb->process, mb->address + offset, mb->data_size);
                    DoubleToString(value, val, sizeof(val) - 1);
                }

                if(limit)
                {
                    AddItemToListView(address, val);
                    --limit;
                }

                else
                {
                    EnableWindow(ListView, false);
                }
            }
        }

        mb = mb->next;
    }

    EnableWindow(ListView, true); 
}

#endif
