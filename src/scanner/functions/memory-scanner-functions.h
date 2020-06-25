#ifndef _CORE_FUNCTIONS_H
#define _CORE_FUNCTIONS_H

// Checks if the bit in MEMORY_BLOCK.match_flag corresponding to an offset in MEMORY_BLOCK.address was cleared in the previous scan. 

bool AddressNotDiscarded(MEMORY_BLOCK *mblock, unsigned long long offset)
{
    MEMORY_BLOCK *mb = mblock;

    if(mb)
    {
        bool not_discarded = (mb->match_flag[(offset) / 8] & (1 << ((offset) % 8))); 
        return not_discarded;
    }

    return false;
}

// Clears the bit in MEMORY_BLOCK.match_flag corresponding to an offset in MEMORY_BLOCK.address.

bool DiscardAddress(MEMORY_BLOCK *mblock, unsigned long long offset)
{
    MEMORY_BLOCK *mb = mblock;

    if(mb)
    {
        mb->match_flag[(offset) / 8] &= ~(1 << ((offset) % 8));
        return true;
    }

    return false;
}

// A set of helper functions that reads/writes the memory at the address specified.

float PeekFloat(HANDLE process, void *address, unsigned short data_size)
{
    float value;

    if(ReadProcessMemory(process, address, (float *)&value, data_size, 0))
    {
        return value;
    }

    return 0.0f;
}

double PeekDouble(HANDLE process, void *address, unsigned short data_size)
{
    double value;

    if(ReadProcessMemory(process, address, (double *)&value, data_size, 0))
    {
        return value;
    }

    return 0.0;
}

long long PeekDecimal(HANDLE process, void *address, unsigned short data_size)
{
    long long value;

    if(ReadProcessMemory(process, address, (long long *)&value, data_size, 0))
    {
        switch(data_size)
        {
            case sizeof(char):

                value = (char)value;
                return value;

                break;

            case sizeof(short):

                value = (short)value;
                return value;

                break;

            case sizeof(int):

                value = (int)value;
                return value;

                break;

            case sizeof(long long):

                value = (long long)value;
                return value;

                break;
        }
    }

    return 0LL;
}

bool PokeFloat(HANDLE process, void *address, float value, unsigned short data_size)
{
    SIZE_T bytes_written;

    if(WriteProcessMemory(process, address, &value, data_size, &bytes_written))
    {
        return ((unsigned short)bytes_written == data_size);
    }

    return false;
}

bool PokeDouble(HANDLE process, void *address, double value, unsigned short data_size)
{
    SIZE_T bytes_written;

    if(WriteProcessMemory(process, address, &value, data_size, &bytes_written))
    {
        return ((unsigned short)bytes_written == data_size);
    }

    return false;
}

bool PokeDecimal(HANDLE process, void *address, long long value, unsigned short data_size)
{
    SIZE_T bytes_written;

    if(WriteProcessMemory(process, address, &value, data_size, &bytes_written))
    {
        return ((unsigned short)bytes_written == data_size);
    }

    return false;
}

// Finds the number of matches from the last scan.

DWORD GetMatchCount(MEMORY_BLOCK *mblock)
{
    DWORD matches = 0;
    MEMORY_BLOCK *mb = mblock;

    while(mb)
    {
        matches += mb->matches;
        mb = mb->next;
    }

    return matches;
}

// Cleans up the memory allocated by CreateMemoryScanner().

void FreeMemoryScanner(MEMORY_BLOCK *mblock)
{
    MEMORY_BLOCK *mb = mblock;

    CloseHandle(mb->process);

    while(mb)
    {
        MEMORY_BLOCK *tmp = mb;

        mb = mb->next;
        if(tmp->buffer) free(tmp->buffer);

        if(tmp->match_flag) free(tmp->match_flag);

        free(tmp);
    }
}

// Resets all previosly filtered addresses.

void ResetScan(MEMORY_BLOCK *mblock, bool reset_pid, bool disable_process_monitor)
{
    MEMORY_BLOCK *mb = mblock;

    EnableWindow(Scan, !reset_pid);

    SelectedProcessOpen = false;
    NumberOfAddressesFrozen = 0;
    SelectedItem = -1;

    MemoryZero(frozen_addresses, sizeof(frozen_addresses));
    MemoryZero(frozen_values, sizeof(frozen_values));

    FirstScanNotRun = true;
    SelectedItem = -1;

    unsigned short i;

    while(mb)
    {
        MemorySet(mb->match_flag, 0xff, mb->size / 8);
        mb->matches = mb->size;
        mb = mb->next;
    }

    SetDlgItemText(MainWindow, ID_VALUE, 0);
    SendMessage(DataSize, CB_RESETCONTENT, 0, 0);
    SendMessage(SearchCondition, CB_RESETCONTENT, 0, 0);
    SendMessage(SearchCondition, CB_ADDSTRING, 0, (LPARAM)search_conditions[SEARCH_EQUALS]);

    if(reset_pid)
    {
        char msg[] =  "*No Process Selected*";

        SendMessage(Pid, WM_SETTEXT, 0, (LPARAM)msg);
        EnableWindow(ChoosePid, true);
    }

    ListView_DeleteAllItems(ListView);

    for(i = 0; i < ARRAYSIZE(data_types); i++)
    {
        SendMessage(DataSize, CB_ADDSTRING, 0, (LPARAM)data_types[i]);
    }

    TerminateThread(FreezeThread, 0);
    WaitForSingleObject(FreezeThread, INFINITE);
    CloseHandle(FreezeThread);

    if(disable_process_monitor)
    {
        TerminateThread(MonitorSelectedProcessThread, 0);
        WaitForSingleObject(MonitorSelectedProcessThread, INFINITE);
        CloseHandle(MonitorSelectedProcessThread);
    }

    TerminateThread(ScanThread, 0);
    WaitForSingleObject(ScanThread, INFINITE);
    CloseHandle(ScanThread);
}

// Constructs and allocates the MEMORY_BLOCK linked list structure.

MEMORY_BLOCK *CreateMemoryBlock(HANDLE process, MEMORY_BASIC_INFORMATION *mbi, unsigned short data_size)
{
    MEMORY_BLOCK *mb = (MEMORY_BLOCK *)malloc(sizeof(*mb));

    if(mb)
    {
        mb->process             = process;
        mb->size                = mbi->RegionSize;
        mb->address             = (unsigned char *)mbi->BaseAddress;
        mb->buffer              = (unsigned char *)malloc(mbi->RegionSize);
        mb->match_flag          = (unsigned char *)malloc(mbi->RegionSize / 8);
        mb->matches             = (unsigned int)mbi->RegionSize;
        mb->data_size           = data_size;
        mb->next                = 0;

        if(mb->match_flag) MemorySet(mb->match_flag, 0xff, mbi->RegionSize / 8);
    }

    return mb;
}

// Finds the initial valid memory information and sets up for UpdateMemoryBlock().

MEMORY_BLOCK *CreateMemoryScanner(DWORD pid, unsigned short data_size)
{
    MEMORY_BLOCK *mblock = 0;
    unsigned char *address = 0;
    MEMORY_BASIC_INFORMATION mbi;

    CurrentProcess = pid;

    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, pid);

    if(process)
    {
        SelectedProcessOpen = true;

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

            address = (unsigned char *)mbi.BaseAddress + mbi.RegionSize;
        }
    }

    else
    {
        EnableWindow(ChoosePid, false);
        ResetScan(0, true, true);
        EnableWindow(MainWindow, false);

        MessageBox(MainWindow, "Error: Failed to open process.", "Error!", MB_OK | MB_ICONERROR);

        EnableWindow(MainWindow, true);
        EnableWindow(ChoosePid, true);
    }

    return mblock;
}

// Filters memory information aquired by CreateMemoryScanner() and subsequent calls to this function.

void UpdateMemoryBlock(MEMORY_BLOCK *mblock, SEARCH_CONDITION condition, TYPE type, double val)
{
    MEMORY_BLOCK *mb = mblock;

    SelectedItem = -1;

    while(mb)
    {
        if(mb->matches)
        {
            static char data_size[256];
            static unsigned char buffer[32 * 1024];
            static unsigned long long total_read, bytes_left, bytes_to_read, bytes_read;

            bytes_left = mb->size;
            total_read = 0;
            mb->matches = 0;

            int selection_id = (int)SendMessage(DataSize, CB_GETCURSEL, 0, 0);

            CopyString(data_size, (char *)data_sizes[selection_id], sizeof(data_size));

            mb->data_size = (unsigned short)StringToInteger(data_size, FMT_INT_DECIMAL);

            while(bytes_left)
            {
                bytes_to_read = (bytes_left > sizeof(buffer)) ? sizeof(buffer) : bytes_left;

                ReadProcessMemory(mb->process, mb->address + total_read, buffer, bytes_to_read, (SIZE_T *)&bytes_read);

                if(bytes_read != bytes_to_read) break;

                double tmpval, prevval;
                unsigned int offset;
                bool match;

                for(offset = 0; offset < bytes_read; offset += mb->data_size)
                {
                    if(AddressNotDiscarded(mb, total_read + offset))
                    {
                        if(type == TYPE_INTEGER)
                        {
                            switch(mb->data_size)
                            {
                                case sizeof(char):

                                    tmpval = *((char *)&buffer[offset]);
                                    prevval = *((char *)&mb->buffer[total_read + offset]);

                                    break;

                                case sizeof(short):

                                    tmpval = *((short *)&buffer[offset]);
                                    prevval = *((short *)&mb->buffer[total_read + offset]);

                                    break;

                                case sizeof(int):

                                    tmpval = *((int *)&buffer[offset]);
                                    prevval = *((int *)&mb->buffer[total_read + offset]);

                                    break;

                                case sizeof(long long):

                                    tmpval = *((long long *)&buffer[offset]);
                                    prevval = *((long long *)&mb->buffer[total_read + offset]);

                                    break;

                            }
                        }

                        else if(type == TYPE_FLOAT)
                        {
                            if(mb->data_size == sizeof(float))
                            {
                                tmpval = *((float *)&buffer[offset]);
                                prevval = *((float *)&mb->buffer[total_read + offset]);
                            }
                        }

                        else if(type == TYPE_DOUBLE)
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

                                match = (tmpval == val);

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
                            mb->matches++;
                        }

                        else
                        {
                            DiscardAddress(mb, total_read + offset);
                        }
                    }
                }

                unsigned int bytes_copied = MemoryCopy(mb->buffer + total_read, buffer, bytes_read);

                bytes_left -= bytes_copied;
                total_read += bytes_copied;
            }

            mb->size = total_read;
        }

        mb = mb->next;
    }
}

// Finds all running processes on machine and finds their process id.

bool GetProcessNameAndID(void)
{
    HANDLE snapshot, process;
    PROCESSENTRY32 pe;

    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if(snapshot != INVALID_HANDLE_VALUE)
    {

        pe.dwSize = sizeof(pe);

        if(!Process32First(snapshot, &pe))
        {
            CloseHandle(snapshot);
            return false;
        }

        do
        {
            if(pe.th32ProcessID == GetCurrentProcessId()) continue;

            CopyString(processes[NumberOfProcesses], pe.szExeFile, sizeof(processes[NumberOfProcesses]));

            process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, pe.th32ProcessID); 

            if(process)
            {
                typedef BOOL (WINAPI *FP_ISWOW64PROCESS) (HANDLE, PBOOL);

                BOOL IsApplication64Bit;

                FP_ISWOW64PROCESS pIsWow64Process = (FP_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

                if(pIsWow64Process && pIsWow64Process(GetCurrentProcess(), &IsApplication64Bit))
                {
                    BOOL IsProcess64Bit;

                    if(pIsWow64Process && pIsWow64Process(process, &IsProcess64Bit))
                    {
                        if(IsApplication64Bit == IsProcess64Bit)
                        {
                            IntegerToString(pe.th32ProcessID, pids[NumberOfProcesses], sizeof(pids[NumberOfProcesses]), FMT_INT_DECIMAL);
                            NumberOfProcesses++; 
                        }
                    }
                }

                else
                {
                    IntegerToString(pe.th32ProcessID, pids[NumberOfProcesses], sizeof(pids[NumberOfProcesses]), FMT_INT_DECIMAL);
                    NumberOfProcesses++; 
                }

                CloseHandle(process);
            }

        } while(Process32Next(snapshot, &pe));

        ProcessCounter = NumberOfProcesses;
        NumberOfProcesses = 0;

        CloseHandle(snapshot);

        return true;
    }

    return false;
}

// Check if selected address in ListView is currently frozen.

bool SelectedAddressFrozen(void)
{
    bool frozen = false;
    unsigned int i;

    char address[256];

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address));

    for(i = 0; i < NumberOfAddressesFrozen; i++)
    {
        if(StringCompare(frozen_addresses[i], address, false))
        {
            frozen = true;
            break;
        }
    }

    return frozen;
}

// Add scan results to user interface.

void DisplayScanResults(MEMORY_BLOCK *mblock)
{
    MEMORY_BLOCK *mb = mblock;
    unsigned int limit = 100;

    while(mb)
    {
        unsigned int offset;

        for(offset = 0; offset < mb->size; offset += mb->data_size)
        {
            if(AddressNotDiscarded(mb, offset))
            {
                char address[256], val[256];

                IntegerToString((unsigned long long)(uintptr_t)mb->address + offset, address, sizeof(address), FMT_INT_HEXADECIMAL);

                if(type == TYPE_INTEGER)
                {
                    long long value = PeekDecimal(mb->process, mb->address + offset, mb->data_size);
                    IntegerToString(value, val, sizeof(val), FMT_INT_DECIMAL);
                }

                else if(type == TYPE_FLOAT)
                {
                    float value = PeekFloat(mb->process, mb->address + offset, mb->data_size);
                    DoubleToString(value, val, sizeof(val));
                }

                else if(type == TYPE_DOUBLE)
                {
                    double value = PeekDouble(mb->process, mb->address + offset, mb->data_size);
                    DoubleToString(value, val, sizeof(val));
                }

                if(limit)
                {
                    AddItemToListView(address, val);
                    limit--;
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

// A thread to monitor if selected process is terminiated.

DWORD WINAPI MonitorSelectedProcess(LPVOID params)
{
    while(SelectedProcessOpen)
    {
        DWORD code = -1;

        SelectedProcessOpen = ((GetExitCodeProcess(scanner->process, &code)) && (code == STILL_ACTIVE));

        if(!SelectedProcessOpen && scanner)
        {
            ResetScan(scanner, true, true);
            break;
        }
    }

    return 0;
}

// A thread to restrict change at frozen addresses.

DWORD WINAPI FreezeAddresses(LPVOID params)
{
    while(scanner)
    {
        if(NumberOfAddressesFrozen)
        {
            unsigned int offset;

            for(offset = 0; offset < NumberOfAddressesFrozen; offset++)
            {
                double value = StringToDouble(frozen_values[offset]);
                unsigned char *address = (unsigned char *)(uintptr_t)StringToInteger(frozen_addresses[offset], FMT_INT_HEXADECIMAL);

                switch(type)
                {
                    case TYPE_FLOAT:

                        CurrentValue = PeekFloat(scanner->process, address, scanner->data_size);

                        if(value != CurrentValue)
                        {
                            PokeFloat(scanner->process, address, value, scanner->data_size);
                        }

                        break;

                    case TYPE_DOUBLE:

                        CurrentValue = PeekDouble(scanner->process, address, scanner->data_size);

                        if(value != CurrentValue)
                        {
                            PokeDouble(scanner->process, address, value, scanner->data_size);
                        }

                        break;

                    case TYPE_INTEGER:

                        CurrentValue = PeekDecimal(scanner->process, address, scanner->data_size);

                        if(value != CurrentValue)
                        {
                            PokeDecimal(scanner->process, address, value, scanner->data_size);
                        }

                        break;
                }
            }
        }
    }

    return 0;
}

// The thread function responsible for performing the scan.

DWORD WINAPI ProcessScan(LPVOID params)
{
    unsigned int matches;
    char pid[256], data_size[256], val[256], condition[256];

    CopyString(pid, PID, sizeof(pid)); 
    SendMessage(Value, WM_GETTEXT, sizeof(val), (LPARAM)val);
    SendMessage(SearchCondition, WM_GETTEXT, sizeof(condition), (LPARAM)condition);

    int selection_id = (int)SendMessage(DataSize, CB_GETCURSEL, 0, 0);
    if(selection_id > -1) CopyString(data_size, (char *)data_sizes[selection_id], sizeof(data_size));

    if((StringLength(pid) && StringLength(data_size) && StringLength(val)) && (!StringCompare(pid, "*No Process Selected*", false)) && (IsDecimal(val)))
    {
        scanner = (scanner) ? scanner : CreateMemoryScanner((unsigned int)StringToInteger(pid, FMT_INT_DECIMAL), (unsigned short)StringToInteger(data_size, FMT_INT_DECIMAL)); 

        if(scanner)
        {
            TerminateThread(MonitorSelectedProcessThread, 0);
            WaitForSingleObject(MonitorSelectedProcessThread, INFINITE); 
            CloseHandle(MonitorSelectedProcessThread);

            DWORD ThreadID;
            MonitorSelectedProcessThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MonitorSelectedProcess, 0, 0, &ThreadID);

            if(MonitorSelectedProcessThread)
            {
                SendMessage(ListView, LVM_DELETEALLITEMS, 0, 0);

                TerminateThread(FreezeThread, 0);
                WaitForSingleObject(FreezeThread, INFINITE);
                CloseHandle(FreezeThread);

                NumberOfAddressesFrozen = 0;
                ScanRunning = true;

                short selected_search_condition = (short)SendMessage(SearchCondition, CB_GETCURSEL, 0, 0);

                if(selected_search_condition > -1)
                {
                    EnableWindow(ChangeValue, false);
                    EnableWindow(Scan, false);
                    EnableWindow(NewScan, false);
                    EnableWindow(ChoosePid, false);
                    EnableWindow(DataSize, false);
                    EnableWindow(Value, false);
                    EnableWindow(SearchCondition, false);

                    type = (selection_id <= 3) ? TYPE_INTEGER : (selection_id == 5) ? TYPE_DOUBLE : TYPE_FLOAT;

                    if(type == TYPE_INTEGER)
                    {
                        switch(selected_search_condition)
                        {
                            case SEARCH_EQUALS:

                                UpdateMemoryBlock(scanner, SEARCH_EQUALS, TYPE_INTEGER, StringToInteger(val, FMT_INT_DECIMAL)); 
                                DisplayScanResults(scanner);

                            break;

                            case SEARCH_INCREASED:

                                UpdateMemoryBlock(scanner, SEARCH_INCREASED, TYPE_INTEGER, 0);
                                DisplayScanResults(scanner);

                            break;

                            case SEARCH_DECREASED:

                                UpdateMemoryBlock(scanner, SEARCH_DECREASED, TYPE_INTEGER, 0);
                                DisplayScanResults(scanner);

                            break;
                        }
                    }

                    else if(type == TYPE_FLOAT)
                    {
                        switch(selected_search_condition)
                        {
                            case SEARCH_EQUALS:

                                UpdateMemoryBlock(scanner, SEARCH_EQUALS, TYPE_FLOAT, (float)StringToDouble(val));
                                DisplayScanResults(scanner);

                            break;

                            case SEARCH_INCREASED:

                                UpdateMemoryBlock(scanner, SEARCH_INCREASED, TYPE_FLOAT, 0);
                                DisplayScanResults(scanner);

                            break;

                            case SEARCH_DECREASED:

                                UpdateMemoryBlock(scanner, SEARCH_DECREASED, TYPE_FLOAT, 0);
                                DisplayScanResults(scanner);

                            break;
                        }
                    }

                    else if(type == TYPE_DOUBLE)
                    {
                        switch(selected_search_condition)
                        {
                            case SEARCH_EQUALS:

                                UpdateMemoryBlock(scanner, SEARCH_EQUALS, TYPE_DOUBLE, StringToDouble(val));
                                DisplayScanResults(scanner);

                            break;

                            case SEARCH_INCREASED:

                                UpdateMemoryBlock(scanner, SEARCH_INCREASED, TYPE_DOUBLE, 0);
                                DisplayScanResults(scanner);

                            break;

                            case SEARCH_DECREASED:

                                UpdateMemoryBlock(scanner, SEARCH_DECREASED, TYPE_DOUBLE, 0);
                                DisplayScanResults(scanner);

                            break;
                        }
                    }

                    matches = GetMatchCount(scanner);

                    if(matches)
                    {
                        DWORD ThreadID;
                        FreezeThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)FreezeAddresses, 0, 0, &ThreadID); 
                    }


                    if(FirstScanNotRun)
                    {
                        SendMessage(SearchCondition, CB_ADDSTRING, 0, (LPARAM)search_conditions[SEARCH_INCREASED]);
                        SendMessage(SearchCondition, CB_ADDSTRING, 0, (LPARAM)search_conditions[SEARCH_DECREASED]);

                        FirstScanNotRun = false;
                    }

                    ScanRunning = false;

                    EnableWindow(Scan, true);
                    EnableWindow(NewScan, true);
                    EnableWindow(ChangeValue, true);
                    EnableWindow(ChoosePid, true);
                    EnableWindow(DataSize, true);
                    EnableWindow(Value, true);
                    EnableWindow(SearchCondition, true);

                    MessageBeep(MB_OK); 
        
                    SetForegroundWindow(MainWindow);
                }
            }
        }
    }

    return 0;
}

#endif
