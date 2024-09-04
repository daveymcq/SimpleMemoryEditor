#ifndef _LOGIC_FUNCTIONS_H
#define _LOGIC_FUNCTIONS_H

/* /////////////////// ArrayList implementation ////////////////////// */

/* Create an ArrayList */

ARRAY_LIST *CreateArrayList(SIZE_T initial_capacity) 
{
    ARRAY_LIST *list = (ARRAY_LIST *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*list));

    if(list)
    {
        list->size = 0;
        list->capacity = initial_capacity;
        list->array = (MEMORY_BLOCK *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, initial_capacity * sizeof(*list->array));

        if(!list->array)
        {
            HeapFree(GetProcessHeap(), 0, list);
            list = null;
        }
    }

    return list;
}

/* Increase the capacity of an ArrayList */

ARRAY_LIST *ArrayListIncreaseCapacity(ARRAY_LIST *list) 
{
    list->capacity *= 2;
    list->array = (MEMORY_BLOCK *)HeapReAlloc(GetProcessHeap(), 
                                              HEAP_ZERO_MEMORY, list->array, 
                                              list->capacity * sizeof(*list->array));

    if(!list->array)
    {
        HeapFree(GetProcessHeap(), 0, list->array);
        list->array = null;
    }

    return list;
}

/* Add Item to an ArrayList */

void ArrayListAdd(ARRAY_LIST *list, MEMORY_BLOCK *mb) 
{
    if(list->size >= list->capacity) 
    {
        ArrayListIncreaseCapacity(list);
    }

    MemoryCopy(&list->array[list->size++], mb, sizeof(*list->array));
}

/* Get Item from an ArrayList */

MEMORY_BLOCK *ArrayListGet(ARRAY_LIST *list, SIZE_T index) 
{
    if(index < list->size) 
    {
        return &list->array[index];
    }

    return null;
}

/* Free resourses used by an ArrayList */

void FreeArrayList(ARRAY_LIST *list) 
{
    list->size = 0;
    list->capacity = 0;
    HeapFree(GetProcessHeap(), 0, list->array);
    list->array = null;
    HeapFree(GetProcessHeap(), 0, list);
    list = null;
}

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

/* Finds the number of matches from the last scan. */

SIZE_T GetMatchCount(MEMORY_BLOCK *mblock)
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

/* A set of helper functions that reads/writes the memory at the address specified. */

real4 PeekFloat(HANDLE process, PVOID address, uint16 data_size)
{
    real4 value;

    if(ReadProcessMemory(process, address, (real4 *)&value, data_size, 0))
    {
        return value;
    }

    return 0.0f;
}

real8 PeekDouble(HANDLE process, PVOID address, uint16 data_size)
{
    real8 value;

    if(ReadProcessMemory(process, address, (real8 *)&value, data_size, 0))
    {
        return value;
    }

    return 0.0;
}

int64 PeekInteger(HANDLE process, PVOID address, uint16 data_size)
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

boolean PokeFloat(HANDLE process, PVOID address, real4 value, uint16 data_size)
{
    SIZE_T bytes_written = 0;

    if(WriteProcessMemory(process, address, &value, data_size, &bytes_written))
    {
        return ((uint16)bytes_written == data_size); 
    }

    return false;
}

boolean PokeDouble(HANDLE process, PVOID address, real8 value, uint16 data_size)
{
    SIZE_T bytes_written = 0;

    if(WriteProcessMemory(process, address, &value, data_size, &bytes_written))
    {
        return ((uint16)bytes_written == data_size); 
    }

    return false;
}

boolean PokeInteger(HANDLE process, PVOID address, int64 value, uint16 data_size)
{
    SIZE_T bytes_written = 0;

    if(WriteProcessMemory(process, address, &value, data_size, &bytes_written))
    {
        return ((uint16)bytes_written == data_size);
    }

    return false;
}

/* Resets all previosly filtered addresses. */

VOID ResetScan(MEMORY_BLOCK *mblock, boolean reset_pid, boolean disable_process_monitor)
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
        mb = ArrayListGet(ArrayList, ++i); 
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

/* Cleans up the memory allocated by CreateMemoryScanner(). */

VOID FreeMemoryScanner(MEMORY_BLOCK *mblock)
{
    SIZE_T i = 0;
    MEMORY_BLOCK *mb = mblock;

    while(mb)
    {
        MEMORY_BLOCK *pmb = mb;
        mb = ArrayListGet(ArrayList, ++i);

        if(pmb->process)
        {
            CloseHandle(pmb->process);
            pmb->process = null;
        }

        if(pmb->buffer)
        {
            HeapFree(GetProcessHeap(), 0, pmb->buffer);
            pmb->buffer = null;
        }

        if(pmb->match_flag)
        {
            HeapFree(GetProcessHeap(), 0, pmb->match_flag);
            pmb->match_flag = null;
        }

//      HeapFree(GetProcessHeap(), 0, pmb);
    }
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

/* A thread to monitor addresses for change. */

DWORD WINAPI FreezeAddresses(VOID)
{
    while(Scanner)
    {
        uint16 i;

        for(i = 0; i < NumberOfAddressesFrozen; i++)
        {
            INTFMT search_number_format;
            real8 value;
            PVOID address;

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

                    CurrentValue = PeekFloat(Scanner->process, address, Scanner->data_size);

                    if(value != CurrentValue)
                    {
                        PokeFloat(Scanner->process, address, value, Scanner->data_size);
                    }

                break;

                case TYPE_DOUBLE:

                    CurrentValue = PeekDouble(Scanner->process, address, Scanner->data_size);

                    if(value != CurrentValue)
                    {
                        PokeDouble(Scanner->process, address, value, Scanner->data_size);
                    }

                break;

                case TYPE_INTEGER:
                    
                    CurrentValue = PeekInteger(Scanner->process, address, Scanner->data_size);

                    if(value != CurrentValue)
                    {
                        PokeInteger(Scanner->process, address, value, Scanner->data_size);
                    }

                break;
            }
        }
    }

    return EXIT_SUCCESS;
}

/* Filters memory information aquired by CreateMemoryScanner() and subsequent calls to this function. */

VOID UpdateMemoryBlock(MEMORY_BLOCK *mblock, SEARCH_CONDITION condition, TYPE Type, real8 value)
{
    SIZE_T i = 0;
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
}

/* The function function responsible for performing the scan. */

DWORD WINAPI ProcessScan(VOID)
{
    static int8 pid[256];
    static int8 data_size[256];
    static int8 val[256];
    static int8 condition[256];
    static int8 status_message[256];

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
                        VOID DisplayScanResults(MEMORY_BLOCK *, INTFMT, uint32);

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

                    StringConcat(IntegerToString(matches, 
                                                 status_message, 
                                                 sizeof(status_message), 
                                                 FMT_INT_DECIMAL), " Matches found!");

                    MessageBoxA(MemoryScannerWindow, 
                                status_message, Title, MB_OK);

                    return EXIT_SUCCESS;
                }
            }
        }
    }

    return EXIT_FAILURE;
}

#endif
