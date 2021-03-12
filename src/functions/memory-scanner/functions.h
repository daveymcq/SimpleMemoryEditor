#ifndef _MEMORY_SCANNER_FUNCTIONS_H
#define _MEMORY_SCANNER_FUNCTIONS_H

/* Checks if the bit in MEMORY_BLOCK.match_flag corresponding to an offset in 
   MEMORY_BLOCK.address was cleared in the previous scan. */

bool AddressNotDiscarded(MEMORY_BLOCK *mblock, uint64 offset)
{
    MEMORY_BLOCK *mb = mblock;

    if(mb)
    {
        bool not_discarded = (mb->match_flag[(offset) / 8] & (1 << ((offset) % 8))); 
        return not_discarded;
    }

    return false;
}

/* Clears the bit in MEMORY_BLOCK.match_flag corresponding to an offset in MEMORY_BLOCK.address. */

bool DiscardAddress(MEMORY_BLOCK *mblock, uint64 offset)
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

void ResetScan(MEMORY_BLOCK *mblock, bool reset_pid, bool disable_process_monitor)
{
    uint16 index;
    MEMORY_BLOCK *mb;

    SelectedProcessOpen = false;
    NumberOfAddressesFrozen = 0;
    FirstScanNotRun = true;
    SelectedItem = -1;
    mb = mblock;

    MemoryZero(FrozenAddresses, sizeof(FrozenAddresses));
    MemoryZero(FrozenValues, sizeof(FrozenValues));

    EnableWindow(Scan, !reset_pid);
    EnableWindow(ChangeValue, false);

    SetDlgItemText(MainWindow, ID_VALUE, 0);

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

    return 0;
}

/* Finds the number of matches from the last scan. */

uint64 GetMatchCount(MEMORY_BLOCK *mblock)
{
    MEMORY_BLOCK *mb = mblock;
    uint64 matches = 0;

    while(mb)
    {
        matches += mb->matches;
        mb = mb->next;
    }

    return matches;
}

/* A set of helper functions that reads/writes the memory at the address specified. */

float PeekFloat(HANDLE process, void *address, uint16 data_size)
{
    float value;

    if(ReadProcessMemory(process, address, (float *)&value, data_size, 0))
    {
        return value;
    }

    return 0.0f;
}

double PeekDouble(HANDLE process, void *address, uint16 data_size)
{
    double value;

    if(ReadProcessMemory(process, address, (double *)&value, data_size, 0))
    {
        return value;
    }

    return 0.0;
}

int64 PeekInteger(HANDLE process, void *address, uint16 data_size)
{
    int64 value = 0;

    if(ReadProcessMemory(process, address, (int64 *)&value, data_size, 0))
    {
        switch(data_size)
        {
            case sizeof(int8):

                value = (int8)value;
                return value;

            break;

            case sizeof(int16):

                value = (int16)value;
                return value; 

            break;
            
            case sizeof(int32):

                value = (int32)value; 
                return value; 

            break;

            case sizeof(int64):

                value = (int64)value;
                return value;

            break;
        }
    }

    return value;
}

bool PokeFloat(HANDLE process, void *address, float value, uint16 data_size)
{
    SIZE_T bytes_written;

    if(WriteProcessMemory(process, address, &value, data_size, &bytes_written))
    {
        return ((uint16)bytes_written == data_size); 
    }

    return false;
}

bool PokeDouble(HANDLE process, void *address, double value, uint16 data_size)
{
    SIZE_T bytes_written;

    if(WriteProcessMemory(process, address, &value, data_size, &bytes_written))
    {
        return ((uint16)bytes_written == data_size); 
    }

    return false;
}

bool PokeInteger(HANDLE process, void *address, int64 value, uint16 data_size)
{
    SIZE_T bytes_written;

    if(WriteProcessMemory(process, address, &value, data_size, &bytes_written))
    {
        return ((uint16)bytes_written == data_size);
    }

    return false;
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
        mb->next                = null;

        if(mb->match_flag) 
        {
            MemorySet(mb->match_flag, 0xff, (mbi->RegionSize / 8));
        }
    }

    return mb;
}

/* Cleans up the memory allocated by CreateMemoryScanner(). */

void FreeMemoryScanner(MEMORY_BLOCK *mblock)
{
    MEMORY_BLOCK *mb = mblock;

    if(mb->process)
    {
        CloseHandle(mb->process);
    }

    while(mb)
    {
        MEMORY_BLOCK *pmb = mb;
        mb = mb->next;

        if(pmb->buffer) 
        {
            HeapFree(GetProcessHeap(), 0, pmb->buffer);
        }

        if(pmb->match_flag) 
        {
            HeapFree(GetProcessHeap(), 0, pmb->match_flag);
        }

        HeapFree(GetProcessHeap(), 0, pmb);
    }
}

/* Finds the initial valid memory information and sets up for UpdateMemoryBlock(). */

MEMORY_BLOCK *CreateMemoryScanner(uint32 pid, uint16 data_size)
{
    MEMORY_BLOCK *mblock;
    MEMORY_BASIC_INFORMATION mbi;
    HANDLE process;
    uint8 *address;

    process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, pid);

    if(process)
    {
        CurrentProcess = pid;
        SelectedProcessOpen = true;
        address = 0;
        mblock = null;

        while(VirtualQueryEx(process, address, &mbi, sizeof(mbi)))
        {
            if((mbi.State & MEM_COMMIT) && (mbi.Protect & MEM_WRITABLE))
            {
                MEMORY_BLOCK *mb = CreateMemoryBlock(process, &mbi, data_size); 

                if(mb)
                {
                    mb->next = mblock;
                    mblock = mb;
                }
            }

            address = (uint8 *)mbi.BaseAddress + mbi.RegionSize;
        }
    }

    return mblock;
}

bool SelectedAddressFrozen(void)
{
    int8 address[256];
    uint32 index;

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address) - 1);

    for(index = 0; index < NumberOfAddressesFrozen; index++)
    {
        if(StringCompare(FrozenAddresses[index], address, false))
        {
            return true;
        }
    }
    
    return false;
}

/* A thread to monitor addresses for change. */

DWORD WINAPI FreezeAddresses(void)
{
    while(Scanner)
    {
        if(NumberOfAddressesFrozen)
        {
            INTFMT search_number_format;
            uint32 offset;
            double value;
            void *address;

            for(offset = 0; offset < NumberOfAddressesFrozen; offset++)
            {
                address = (void *)(uintptr_t)StringToInteger(FrozenAddresses[offset], FMT_INT_HEXADECIMAL);
                value = StringToDouble(FrozenValues[offset]);

                if((IsNumeric(FrozenValues[offset])) && ((FrozenValues[offset][0] == '0') && (FrozenValues[offset][1] == 'x')))
                {
                    search_number_format = FMT_INT_HEXADECIMAL;
                }

                else if(IsNumeric(FrozenValues[offset]) && ((FrozenValues[offset][0] != '0') && (FrozenValues[offset][1] != 'x')))
                {
                    search_number_format = FMT_INT_DECIMAL;
                }

                else
                {
                    search_number_format = (INTFMT)0;
                }

                if(search_number_format)
                {
                    value = StringToInteger(FrozenValues[offset], search_number_format);
                }

                switch(Type)
                {
                    case TYPE_FLOAT:

                        CurrentValue = PeekFloat(Scanner->process, address, Scanner->data_size);

                        if(value != CurrentValue)
                        {
                            PokeFloat(Scanner->process, address, value, Scanner->data_size);
                            return 0;
                        }

                    break;

                    case TYPE_DOUBLE:

                        CurrentValue = PeekDouble(Scanner->process, address, Scanner->data_size);

                        if(value != CurrentValue)
                        {
                            PokeDouble(Scanner->process, address, value, Scanner->data_size);
                            return 0;
                        }

                    break;

                    case TYPE_INTEGER:
                        
                        CurrentValue = PeekInteger(Scanner->process, address, Scanner->data_size);

                        if(value != CurrentValue)
                        {
                            PokeInteger(Scanner->process, address, value, Scanner->data_size);
                            return 0;
                        }

                    break;
                }
            }
        }
    }

    return -1;
}

/* Filters memory information aquired by CreateMemoryScanner() and subsequent calls to this function. */

void UpdateMemoryBlock(MEMORY_BLOCK *mblock, SEARCH_CONDITION condition, TYPE Type, double value)
{
    MEMORY_BLOCK *mb = mblock;

    while(mb)
    {
        if(mb->matches)
        {
            static int8 buffer[256 * 1024];

            static uint64 total_read;
            static uint64 bytes_left;
            static uint64 bytes_to_read;
            static uint64 bytes_read;

            int32 selection_id;
            uint64 bytes_copied;

            total_read = 0;
            mb->matches = 0;
            bytes_left = mb->size;

            selection_id = (int32)SendMessageA(DataSize, CB_GETCURSEL, 0, 0);
            mb->data_size = (uint16)StringToInteger(DataSizes[selection_id], FMT_INT_DECIMAL);

            while(bytes_left)
            {
                uint64 offset;
                double tmpval, prevval;
                bool match;

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
                            if(mb->data_size == sizeof(float))
                            {
                                tmpval = *((float *)&buffer[offset]);
                                prevval = *((float *)&mb->buffer[total_read + offset]);
                            }
                        }

                        else if(Type == TYPE_DOUBLE)
                        {
                            if(mb->data_size == sizeof(double))
                            {
                                tmpval = *((double *)&buffer[offset]);
                                prevval = *((double *)&mb->buffer[total_read + offset]);
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

        mb = mb->next; 
    }
}

/* Add scan results to user interface. */

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

/* The thread function responsible for performing the scan. */

DWORD WINAPI ProcessScan(void)
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

    if((IsNumeric(val)) && ((val[0] == '0') && (val[1] == 'x')))
    {
        search_number_format = FMT_INT_HEXADECIMAL;
    }

    if((StringLength(pid) && StringLength(data_size) && StringLength(val)) && (!StringCompare(pid, "*No Process Selected*", false)))
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

                                UpdateMemoryBlock(Scanner, SEARCH_EQUALS, TYPE_FLOAT, (float)StringToDouble(val));
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
                    EnableWindow(MainWindow, false);
                    EnableWindow(MainWindow, true);

                    SetForegroundWindow(MainWindow);

                    ScanRunning = false;

                    return (DWORD)MessageBeep(MB_OK);
                }
            }
        }
    }

    return -1;
}

/* Once an address is found, this function updates the value at that address. */

bool UpdateValue(void)
{
    int8 address[256];
    int8 value[256];
    int8 buffer[256];

    LVITEM item;
    INTFMT search_number_format;

    void *addr;
    double val;

    item.mask = LVIF_TEXT;
    item.iItem = SelectedItem;
    item.iSubItem = 1;
    item.pszText = buffer;

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address) - 1);
    SendMessageA(ChangeValueWindowNewValue, WM_GETTEXT, sizeof(value) - 1, (LPARAM)value);
    SendMessageA(Value, WM_GETTEXT, sizeof(buffer) - 1, (LPARAM)buffer);
    
    ListView_SetItem(ListView, &item);

    if((IsNumeric(value)) && ((value[0] == '0') && (value[1] == 'x')))
    {
        search_number_format = FMT_INT_HEXADECIMAL;
    }

    else if(IsNumeric(value) && ((value[0] != '0') && (value[1] != 'x')))
    {
        search_number_format = FMT_INT_DECIMAL;
    }

    if(Type == TYPE_FLOAT)
    {
        addr = (void *)(uintptr_t)StringToInteger(address, FMT_INT_HEXADECIMAL);
        val = StringToDouble(value);

        if(PokeFloat(Scanner->process, addr, (float)val, Scanner->data_size))
        {
            float current_value;

            DestroyWindow(ChangeValueWindow);
            SetForegroundWindow(MainWindow);
            SendMessageA(ChangeValueWindowNewValue, WM_GETTEXT, sizeof(value) - 1, (LPARAM)value);

            current_value = PeekFloat(Scanner->process, addr, Scanner->data_size);

            DoubleToString((double)current_value, value, sizeof(value) - 1);
            ListView_SetItemText(ListView, SelectedItem, 1, value);

            return true;
        }
    }

    else if(Type == TYPE_DOUBLE)
    {
        addr = (void *)(uintptr_t)StringToInteger(address, FMT_INT_HEXADECIMAL);
        val = StringToDouble(value);

        if(PokeDouble(Scanner->process, addr, val, Scanner->data_size))
        {
            double current_value;

            DestroyWindow(ChangeValueWindow);
            SetForegroundWindow(MainWindow);
            SendMessageA(ChangeValueWindowNewValue, WM_GETTEXT, sizeof(value) - 1, (LPARAM)value);

            current_value = PeekFloat(Scanner->process, addr, Scanner->data_size);

            DoubleToString(current_value, value, sizeof(value) - 1);
            ListView_SetItemText(ListView, SelectedItem, 1, value);

            return true;
        }
    }

    else if(Type == TYPE_INTEGER)
    {
        addr = (void *)(uintptr_t)StringToInteger(address, FMT_INT_HEXADECIMAL);
        val = StringToInteger(value, search_number_format);

        if(PokeInteger(Scanner->process, addr, (int64)val, Scanner->data_size))
        {
            int64 current_value;

            DestroyWindow(ChangeValueWindow);
            SetForegroundWindow(MainWindow);
            SendMessageA(ChangeValueWindowNewValue, WM_GETTEXT, sizeof(value) - 1, (LPARAM)value);

            current_value = PeekInteger(Scanner->process, addr, Scanner->data_size);

            if(search_number_format == FMT_INT_HEXADECIMAL)
            {
                value[0] = '0';
                value[1] = 'x';

                IntegerToString(current_value, value + 2, sizeof(value) - 3, search_number_format);
            }

            else if(search_number_format == FMT_INT_DECIMAL)
            {
                IntegerToString(current_value, value, sizeof(value) - 1, search_number_format);
            }

            ListView_SetItemText(ListView, SelectedItem, 1, value);

            return true;
        }
    }

    return false;
}

#endif
