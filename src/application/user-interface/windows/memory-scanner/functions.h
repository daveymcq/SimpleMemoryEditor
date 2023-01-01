#ifndef _MEMORY_SCANNER_WINDOW_FUNCTIONS_H
#define _MEMORY_SCANNER_WINDOW_FUNCTIONS_H

/* Check if the selected address is frozen by comparing it against FrozenAddress */

boolean IsAddressFrozen(string address)
{
    uint16 i;

    for(i = 0; i < FREEZE_LIMIT; i++)
    {
       if(StringCompare(FrozenAddresses[i], address, false)) return true;
    }

    return false;
}

/* Add scan results to ListView control. */

VOID AddItemToListView(string address, string value)
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

VOID ResetScan(MEMORY_BLOCK *mblock, boolean reset_pid, boolean disable_process_monitor)
{
    uint16 index;
    MEMORY_BLOCK *mb;

    SelectedProcessOpen = false;
    NumberOfAddressesFrozen = 0;
    FirstScanNotRun = true;
    AddressFrozen = false;
    SelectedItem = -1;
    mb = mblock;

    MemoryZero(FrozenAddresses, sizeof(FrozenAddresses));
    MemoryZero(FrozenValues, sizeof(FrozenValues));

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
        string msg = (string)"*No Process Selected*";
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

    EnableWindow(ListView, false);
}

/* Calls ResetScan() if the selected thread terminates. */

DWORD WINAPI MonitorSelectedProcess(VOID)
{
    while(Scanner)
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

VOID DisplayScanResults(MEMORY_BLOCK *mblock, INTFMT display_format, uint32 display_limit)
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
                    real4 value = PeekFloat(mb->process, mb->address + offset, mb->data_size);
                    DoubleToString((real8)value, val, sizeof(val) - 1);
                }

                else if(Type == TYPE_DOUBLE)
                {
                    real8 value = PeekDouble(mb->process, mb->address + offset, mb->data_size);
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

/* The function function responsible for performing the scan. */

DWORD WINAPI ProcessScan(VOID)
{
    static int8 pid[256];
    static int8 data_size[256];
    static int8 val[256];
    static int8 condition[256];

    INTFMT search_number_format;
    int32 selection_id;
    uint64 matches;

    search_number_format = FMT_INT_DECIMAL;
    selection_id = (int32)SendMessageA(DataSize, CB_GETCURSEL, 0, 0);

    CopyString(pid, SelectedPid, sizeof(pid) - 1); 

    SendMessageA(Value, WM_GETTEXT, sizeof(val) - 1, (LPARAM)val);
    SendMessageA(SearchCondition, WM_GETTEXT, sizeof(condition) - 1, (LPARAM)condition);

    if(selection_id > -1) 
    {
        CopyString(data_size, (string)DataSizes[selection_id], sizeof(data_size) - 1);
    }

    if((val[0] == '0') && (val[1] == 'x'))
    {
        search_number_format = FMT_INT_HEXADECIMAL;
    }

    if((IsNumeric(val)) && ((StringLength(pid) && StringLength(data_size) && StringLength(val)) && (!StringCompare(pid, (string)"*No Process Selected*", false))))
    {
        Scanner = (Scanner) ? Scanner : CreateMemoryScanner((uint32)StringToInteger(pid, FMT_INT_DECIMAL), (uint16)StringToInteger(data_size, FMT_INT_DECIMAL)); 

        if(Scanner)
        {
            if(MonitorSelectedProcessThread)
            {
                int8 selected_search_condition;
                NumberOfAddressesFrozen = 0;
                ScanRunning = true;
                selected_search_condition = (int8)SendMessageA(SearchCondition, CB_GETCURSEL, 0, 0);

                if(selected_search_condition > -1)
                {
                    SelectedItem = -1;
                    Type = (selection_id <= 3) ? TYPE_INTEGER : (selection_id == 5) ? TYPE_DOUBLE : TYPE_FLOAT;

                    SendMessageA(ListView, LVM_DELETEALLITEMS, 0, 0);

                    EnableWindow(ChangeValue, false);
                    EnableWindow(Scan, false);
                    EnableWindow(NewScan, false);
                    EnableWindow(ChoosePid, false);
                    EnableWindow(DataSize, false);
                    EnableWindow(Value, false);
                    EnableWindow(SearchCondition, false);

                    if(Type == TYPE_INTEGER)
                    {
                        switch(selected_search_condition)
                        {
                            case SEARCH_EQUALS:

                                UpdateMemoryBlock(Scanner, SEARCH_EQUALS, TYPE_INTEGER, StringToInteger(val, search_number_format)); 
                                DisplayScanResults(Scanner, search_number_format, 100);

                            break;

                            case SEARCH_INCREASED:

                                UpdateMemoryBlock(Scanner, SEARCH_INCREASED, TYPE_INTEGER, 0);
                                DisplayScanResults(Scanner, search_number_format, 100);

                            break;

                            case SEARCH_DECREASED:

                                UpdateMemoryBlock(Scanner, SEARCH_DECREASED, TYPE_INTEGER, 0);
                                DisplayScanResults(Scanner, search_number_format, 100);

                            break;
                        }
                    }

                    else if(Type == TYPE_FLOAT)
                    {
                        switch(selected_search_condition)
                        {
                            case SEARCH_EQUALS:

                                UpdateMemoryBlock(Scanner, SEARCH_EQUALS, TYPE_FLOAT, (real4)StringToDouble(val));
                                DisplayScanResults(Scanner, search_number_format, 100);

                            break;

                            case SEARCH_INCREASED:

                                UpdateMemoryBlock(Scanner, SEARCH_INCREASED, TYPE_FLOAT, 0);
                                DisplayScanResults(Scanner, search_number_format, 100);

                            break;

                            case SEARCH_DECREASED:

                                UpdateMemoryBlock(Scanner, SEARCH_DECREASED, TYPE_FLOAT, 0);
                                DisplayScanResults(Scanner, search_number_format, 100);

                            break;
                        }
                    }

                    else if(Type == TYPE_DOUBLE)
                    {
                        switch(selected_search_condition)
                        {
                            case SEARCH_EQUALS:

                                UpdateMemoryBlock(Scanner, SEARCH_EQUALS, TYPE_DOUBLE, StringToDouble(val));
                                DisplayScanResults(Scanner, search_number_format, 100);

                            break;

                            case SEARCH_INCREASED:

                                UpdateMemoryBlock(Scanner, SEARCH_INCREASED, TYPE_DOUBLE, 0);
                                DisplayScanResults(Scanner, search_number_format, 100);

                            break;

                            case SEARCH_DECREASED:

                                UpdateMemoryBlock(Scanner, SEARCH_DECREASED, TYPE_DOUBLE, 0);
                                DisplayScanResults(Scanner, search_number_format, 100);

                            break;
                        }
                    }

                    matches = GetMatchCount(Scanner);

                    if(matches)
                    {
                        if(FreezeThread && TerminateThread(FreezeThread, 0))
                        {
                            WaitForSingleObject(FreezeThread, INFINITE); 
                            CloseHandle(FreezeThread);
                        }

                        FreezeThread = CreateThread(null, null, (LPTHREAD_START_ROUTINE)FreezeAddresses, null, null, null); 
                    }

                    if(FirstScanNotRun)
                    {
                        FirstScanNotRun = false;

                        SendMessageA(SearchCondition, CB_ADDSTRING, 0, (LPARAM)SearchConditions[SEARCH_INCREASED]);
                        SendMessageA(SearchCondition, CB_ADDSTRING, 0, (LPARAM)SearchConditions[SEARCH_DECREASED]);
                    }

                    EnableWindow(Scan, true);
                    EnableWindow(NewScan, true);
                    EnableWindow(ChoosePid, true);
                    EnableWindow(DataSize, true);
                    EnableWindow(Value, true);
                    EnableWindow(SearchCondition, true);
                    EnableWindow(MemoryScannerWindow, false);
                    EnableWindow(MemoryScannerWindow, true);

                    SetForegroundWindow(MemoryScannerWindow);
                    MessageBeep(MB_OK);

                    ScanRunning = false;

                    return EXIT_SUCCESS;
                }
            }
        }
    }

    return EXIT_FAILURE;
}

#endif
