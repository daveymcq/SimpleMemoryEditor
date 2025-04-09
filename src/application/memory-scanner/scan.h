#ifndef _SCAN_H
#define _SCAN_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Memory Scanner Logic                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////

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

/* Checks if the bit in MEMORY_BLOCK.match_flag corresponding to an offset in 
   MEMORY_BLOCK.address was cleared in the previous scan. */

boolean AddressNotDiscarded(MEMORY_BLOCK *mblock, uint64 offset)
{
    MEMORY_BLOCK *mb = mblock;

    if(mb)
    {
        boolean not_discarded = (mb->match_flag[(offset) / 8] & (1 << ((offset) % 8))); 
        return not_discarded;
    }

    return false;
}

/* Clears the bit in MEMORY_BLOCK.match_flag corresponding to an offset in MEMORY_BLOCK.address. */

boolean DiscardAddress(MEMORY_BLOCK *mblock, uint64 offset)
{
    MEMORY_BLOCK *mb = mblock;

    if(mb)
    {
        mb->match_flag[(offset) / 8] &= ~(1 << ((offset) % 8));
        return true;
    }

    return false;
}

/* Resets all previosly filtered addresses. */

void ResetScan(MEMORY_BLOCK *mblock, boolean reset_pid, boolean disable_process_monitor)
{
    MEMORY_BLOCK *mb = mblock;
    uint16 index = 0;
    SIZE_T i = 0;

    SelectedProcessOpen = false;
    NumberOfAddressesFrozen = 0;
    FirstScanNotRun = true;
    AddressFrozen = false;
    SelectedItem = -1;

    MemoryZero(FrozenAddresses, sizeof(FrozenAddresses));
    MemoryZero(FrozenValues, sizeof(FrozenValues));

    EnableWindow(Scan, !reset_pid);
    EnableWindow(NewScan, false);

    SetDlgItemText(MemoryScannerWindow, ID_VALUE, 0);
    SendMessageA(SearchCondition, CB_RESETCONTENT, 0, 0);
    SendMessageA(SearchCondition, CB_ADDSTRING, 0, (LPARAM)SearchConditions[SEARCH_EQUALS]);

    ListView_DeleteAllItems(ListView);

    while(mb)
    {
        MemorySet(mb->match_flag, 0xff, mb->size / 8); 
        mb->matches = mb->size;
        mb = ArrayListGet(ArrayList, ++i); 
    }

    SendMessageA(SearchCondition, CB_SETCURSEL, (WPARAM)0, 0);
    SendMessageA(DataSize, CB_SETCURSEL, (WPARAM)2, 0);

    if(reset_pid)
    {
        SendMessageA(SearchCondition, CB_RESETCONTENT, 0, 0);
        SendMessageA(DataSize, CB_RESETCONTENT, 0, 0);
        EnableWindow(SearchCondition, false);
        EnableWindow(ChoosePid, true);
        EnableWindow(DataSize, false);
        EnableWindow(Value, false);
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

/* A thread to monitor addresses for change. */

DWORD WINAPI FreezeSelectedScannedAddresses(void)
{
    while(Scanner)
    {
        uint16 i;

        WaitForSingleObject(Mutex, INFINITE);

        for(i = 0; i < NumberOfAddressesFrozen; i++)
        {
            INTFMT search_number_format;
            real8 value;
            void *address;

            address = (PVOID) (uintptr_t)StringToInteger(FrozenAddresses[i], FMT_INT_HEXADECIMAL);
            value = StringToDouble(FrozenValues[i]);

            if((IsNumeric(FrozenValues[i])) && ((FrozenValues[i][0] == '0') && (FrozenValues[i][1] == 'x')))
            {
                search_number_format = FMT_INT_HEXADECIMAL;
            }

            else if(IsNumeric(FrozenValues[i]) && ((FrozenValues[i][0] != '0') && (FrozenValues[i][1] != 'x')))
            {
                search_number_format = FMT_INT_DECIMAL;
            }

            else
            {
                search_number_format = (INTFMT)0;
            }

            if(search_number_format)
            {
                value = StringToInteger(FrozenValues[i], search_number_format);
            }

            switch(Type)
            {
                case TYPE_FLOAT:

                    CurrentValue = ReadFloat(Scanner->process, address);

                    if(value != CurrentValue)
                    {
                        WriteFloat(Scanner->process, address, value);
                    }

                break;

                case TYPE_DOUBLE:

                    CurrentValue = ReadDouble(Scanner->process, address);

                    if(value != CurrentValue)
                    {
                        WriteDouble(Scanner->process, address, value);
                    }

                break;

                case TYPE_INTEGER:
                    
                    CurrentValue = ReadInteger(Scanner->process, address, Scanner->data_size);

                    if(value != CurrentValue)
                    {
                        WriteInteger(Scanner->process, address, value, Scanner->data_size);
                    }

                break;
            }
        }

        ReleaseMutex(Mutex);
    }

    return EXIT_SUCCESS;
}

/* Constructs and allocates the MEMORY_BLOCK linked list structure. */

MEMORY_BLOCK *CreateMemoryBlock(HANDLE process, MEMORY_BASIC_INFORMATION *mbi, uint16 data_size)
{
    MEMORY_BLOCK *mb = (MEMORY_BLOCK *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*mb));

    if(mb)
    {
        mb->process             = process;
        mb->size                = (uint64)mbi->RegionSize;
        mb->address             = (uint8 *)mbi->BaseAddress;
        mb->buffer              = (uint8 *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, mbi->RegionSize);
        mb->match_flag          = (uint8 *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (mbi->RegionSize / 8));
        mb->matches             = (uint64)mbi->RegionSize;
        mb->data_size           = data_size; 

        if(mb->match_flag) 
        {
            MemorySet(mb->match_flag, 0xff, (mbi->RegionSize / 8));
        }
    }

    return mb;
}

/* Finds the initial valid memory information and sets up for UpdateMemoryBlock(). */

MEMORY_BLOCK *CreateMemoryScanner(uint32 pid, uint16 data_size)
{
    MEMORY_BASIC_INFORMATION mbi;
    MEMORY_BLOCK *mblock;
    HANDLE process;
    uint8 *address;

    process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, pid);

    if(process)
    {
        SelectedProcessOpen = true;
        CurrentProcess = pid;
        mblock = null;
        address = 0;

        while(VirtualQueryEx(process, address, &mbi, sizeof(mbi)))
        {
            if((mbi.State & MEM_COMMIT) && (mbi.Protect & WRITABLE_MEMORY))
            {
                MEMORY_BLOCK *mb = CreateMemoryBlock(process, &mbi, data_size); 

                if(mb)
                {
                    ArrayList = (ArrayList) ? ArrayList : CreateArrayList(sizeof(*mb));
                    ArrayListAdd(ArrayList, mb);
                }
            }

            address = (uint8 *)mbi.BaseAddress + mbi.RegionSize;
        }
    }

    return ArrayListGet(ArrayList, 0);
}

/* Filters memory information aquired by CreateMemoryScanner() and subsequent calls to this function. */

void UpdateMemoryBlock(MEMORY_BLOCK *mblock, SEARCH_CONDITION condition, TYPE Type, real8 value)
{
    SIZE_T i = 0;
    MEMORY_BLOCK *mb = mblock;

    WaitForSingleObject(Mutex, INFINITE);

    while(mb)
    {
        if(mb->matches)
        {
            static int8 buffer[8192];

            static uint64 total_read;
            static uint64 bytes_left;
            static uint64 bytes_to_read;
            static uint64 bytes_read;

            int32 selection_id;

            total_read = 0;
            mb->matches = 0;
            bytes_left = mb->size;

            selection_id = (int32)SendMessageA(DataSize, CB_GETCURSEL, 0, 0);
            mb->data_size = (uint16)StringToInteger(DataSizes[selection_id], FMT_INT_DECIMAL);

            while(bytes_left)
            {
                uint64 offset;
                real8 tmpval, prevval;
                boolean match;

                bytes_to_read = (bytes_left > sizeof(buffer)) ? sizeof(buffer) : bytes_left;

                ReadProcessMemory(mb->process, mb->address + total_read, buffer, bytes_to_read, (SIZE_T *)&bytes_read);

                if(bytes_read != bytes_to_read) break;

                for(offset = 0; offset < bytes_read; offset += mb->data_size)
                {
                    if(AddressNotDiscarded(mb, total_read + offset)) 
                    {
                        if(Type == TYPE_INTEGER)
                        {
                            switch(mb->data_size)
                            {
                                case sizeof(int8):

                                    tmpval = *((int8 *)&buffer[offset]);
                                    prevval = *((int8 *)&mb->buffer[total_read + offset]);

                                break;

                                case sizeof(int16):

                                    tmpval = *((int16 *)&buffer[offset]);
                                    prevval = *((int16 *)&mb->buffer[total_read + offset]);

                                break;

                                case sizeof(int32):

                                    tmpval = *((int32 *)&buffer[offset]);
                                    prevval = *((int32 *)&mb->buffer[total_read + offset]);

                                break;

                                case sizeof(int64):

                                    tmpval = *((int64 *)&buffer[offset]);
                                    prevval = *((int64 *)&mb->buffer[total_read + offset]);

                                break;
                            }
                        }

                        else if(Type == TYPE_FLOAT)
                        {
                            if(mb->data_size == sizeof(real4))
                            {
                                tmpval = *((real4 *)&buffer[offset]);
                                prevval = *((real4 *)&mb->buffer[total_read + offset]);
                            }
                        }

                        else if(Type == TYPE_DOUBLE)
                        {
                            if(mb->data_size == sizeof(real8))
                            {
                                tmpval = *((real8 *)&buffer[offset]);
                                prevval = *((real8 *)&mb->buffer[total_read + offset]);
                            }
                        }

                        switch(condition)
                        {
                            case SEARCH_EQUALS:

                                match = (tmpval == value);

                            break;

                            case SEARCH_INCREASED:

                                match = (tmpval > prevval);

                            break;

                            case SEARCH_DECREASED:

                                match = (tmpval < prevval);
                                
                            break;
                        }

                        if(match)
                        {
                            ++mb->matches;
                        }

                        else 
                        {
                            DiscardAddress(mb, total_read + offset);
                        }
                    }
                }

                MemoryCopy(mb->buffer + total_read, buffer, bytes_read);

                bytes_left -= bytes_read;
                total_read += bytes_read;
            }

            mb->size = total_read;
        }

        mb = ArrayListGet(ArrayList, ++i); 
    }

    ReleaseMutex(Mutex);
}

/* Finds the number of matches from the last scan. */

SIZE_T GetMatchCountFromLastScan(MEMORY_BLOCK *mblock)
{
    MEMORY_BLOCK *mb = mblock;
    SIZE_T matches = 0;
    SIZE_T i = 0;

    while(mb)
    {
        matches += mb->matches;
        mb = ArrayListGet(ArrayList, ++i); 
    }

    return matches;
}

/* The function function responsible for performing the scan. */

DWORD WINAPI CreateNewScan(void)
{
    static int8 pid[256];
    static int8 val[256];
    static int8 data_size[256];
    static int8 condition[256];
    static int8 status_message[256];

    uint64 matches;
    int32 selection_id;
    INTFMT search_number_format;

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

    if((IsNumeric(val)) && ((StringLength(pid) && StringLength(data_size) && StringLength(val) && StringLength(data_size))))
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

                    EnableWindow(SearchCondition, false);
                    EnableWindow(ChoosePid, false);
                    EnableWindow(DataSize, false);
                    EnableWindow(NewScan, false);
                    EnableWindow(Value, false);
                    EnableWindow(Scan, false);

                    if(Type == TYPE_INTEGER)
                    {
                        void DisplayScanResults(MEMORY_BLOCK *, INTFMT, uint32);

                        switch(selected_search_condition)
                        {
                            case SEARCH_EQUALS:

                                UpdateMemoryBlock(Scanner, SEARCH_EQUALS, TYPE_INTEGER, StringToInteger(val, search_number_format)); 
                                DisplayScanResults(Scanner, search_number_format, FREEZE_LIMIT);

                            break;

                            case SEARCH_INCREASED:

                                UpdateMemoryBlock(Scanner, SEARCH_INCREASED, TYPE_INTEGER, 0);
                                DisplayScanResults(Scanner, search_number_format, FREEZE_LIMIT);

                            break;

                            case SEARCH_DECREASED:

                                UpdateMemoryBlock(Scanner, SEARCH_DECREASED, TYPE_INTEGER, 0);
                                DisplayScanResults(Scanner, search_number_format, FREEZE_LIMIT);

                            break;
                        }
                    }

                    else if(Type == TYPE_FLOAT)
                    {
                        switch(selected_search_condition)
                        {
                            case SEARCH_EQUALS:

                                UpdateMemoryBlock(Scanner, SEARCH_EQUALS, TYPE_FLOAT, (real4)StringToDouble(val));
                                DisplayScanResults(Scanner, search_number_format, FREEZE_LIMIT);

                            break;

                            case SEARCH_INCREASED:

                                UpdateMemoryBlock(Scanner, SEARCH_INCREASED, TYPE_FLOAT, 0);
                                DisplayScanResults(Scanner, search_number_format, FREEZE_LIMIT);

                            break;

                            case SEARCH_DECREASED:

                                UpdateMemoryBlock(Scanner, SEARCH_DECREASED, TYPE_FLOAT, 0);
                                DisplayScanResults(Scanner, search_number_format, FREEZE_LIMIT);

                            break;
                        }
                    }

                    else if(Type == TYPE_DOUBLE)
                    {
                        switch(selected_search_condition)
                        {
                            case SEARCH_EQUALS:

                                UpdateMemoryBlock(Scanner, SEARCH_EQUALS, TYPE_DOUBLE, StringToDouble(val));
                                DisplayScanResults(Scanner, search_number_format, FREEZE_LIMIT);

                            break;

                            case SEARCH_INCREASED:

                                UpdateMemoryBlock(Scanner, SEARCH_INCREASED, TYPE_DOUBLE, 0);
                                DisplayScanResults(Scanner, search_number_format, FREEZE_LIMIT);

                            break;

                            case SEARCH_DECREASED:

                                UpdateMemoryBlock(Scanner, SEARCH_DECREASED, TYPE_DOUBLE, 0);
                                DisplayScanResults(Scanner, search_number_format, FREEZE_LIMIT);

                            break;
                        }
                    }

                    matches = GetMatchCountFromLastScan(Scanner);

                    if(matches)
                    {
                        if(FreezeThread && TerminateThread(FreezeThread, 0))
                        {
                            WaitForSingleObject(FreezeThread, INFINITE); 
                            CloseHandle(FreezeThread);
                        }

                        FreezeThread = CreateThread(null, null, (LPTHREAD_START_ROUTINE)FreezeSelectedScannedAddresses, null, null, null); 
                    }

                    if(FirstScanNotRun)
                    {
                        FirstScanNotRun = false;

                        SendMessageA(SearchCondition, CB_ADDSTRING, 0, (LPARAM)SearchConditions[SEARCH_INCREASED]);
                        SendMessageA(SearchCondition, CB_ADDSTRING, 0, (LPARAM)SearchConditions[SEARCH_DECREASED]);
                    }

                    SetForegroundWindow(MemoryScannerWindow);
                    EnableWindow(MemoryScannerWindow, false);
                    EnableWindow(MemoryScannerWindow, true);
                    EnableWindow(SearchCondition, true);
                    EnableWindow(ChoosePid, true);
                    EnableWindow(DataSize, true);
                    EnableWindow(NewScan, true);
                    EnableWindow(Value, true);
                    EnableWindow(Scan, true);
                    ScanRunning = false;
                    MessageBeep(MB_OK);

                    StringConcat(IntegerToString(matches, 
                                                 status_message, 
                                                 sizeof(status_message), 
                                                 FMT_INT_DECIMAL), " Matches found!");

                    return MessageBoxA(MemoryScannerWindow, 
                                        status_message, 
                                        Title, MB_OK);
                }
            }
        }
    }

    return EXIT_FAILURE;
}

/* Cleans up the memory allocated by CreateMemoryScanner(). */

void FreeMemoryScanner(MEMORY_BLOCK *mblock)
{
    MEMORY_BLOCK *mb = (mblock) ? mblock : ArrayListGet(ArrayList, 0);

    while(mb)
    {
        if(mb->process)
        {
            CloseHandle(mb->process);
            mb->process = null;
        }

        if(mb->buffer)
        {
            HeapFree(GetProcessHeap(), 0, mb->buffer);
            mb->buffer = null;
        }

        if(mb->match_flag)
        {
            HeapFree(GetProcessHeap(), 0, mb->match_flag);
            mb->match_flag = null;
        }

        mb = ArrayListGet(ArrayList, mb->size + 1);
    }
}

/* Add scan results to memory scanner window. */

void DisplayScanResults(MEMORY_BLOCK *mblock, INTFMT display_format, uint32 display_limit)
{
    SIZE_T i = 0;
    MEMORY_BLOCK *mb = mblock;
    uint32 limit = (GetMatchCountFromLastScan(mb) > display_limit) ? display_limit : (uint32)GetMatchCountFromLastScan(mb);

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
                    int64 value = ReadInteger(mb->process, mb->address + offset, mb->data_size);

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
                    real4 value = ReadFloat(mb->process, mb->address + offset);
                    DoubleToString((real8)value, val, sizeof(val) - 1);
                }

                else if(Type == TYPE_DOUBLE)
                {
                    real8 value = ReadDouble(mb->process, mb->address + offset);
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

        mb = ArrayListGet(ArrayList, ++i); 
    }

    EnableWindow(ListView, true); 
}

#endif