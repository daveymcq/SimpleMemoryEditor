#ifndef _CORE_FUNCTIONS_H
#define _CORE_FUNCTIONS_H

// Debugging code //

void __DEBUG_INT(long long X)
{
    char msg[1024];
    _snprintf(msg, sizeof(msg), "%lld", X);
    MessageBox(0, msg, 0, MB_OK);
}

// Initialize local variables.

void Init(void)
{
    Width = 625;
    Height = 425;
    FirstScanNotRun = TRUE;
    SelectedItem = -1;
}


// Find length of string.

unsigned int StringLength(char *str)
{
    unsigned int length = 0;
    char *pstr = str;

    while(*pstr)
    {
        ++length;
        pstr++;
    }

    return length;
}


// Absolute value


unsigned int Abs(int number)
{
    unsigned int result = 0;
    
    if(number >= 0)
    {
        result = number;
    }
    else 
    {
        result = -number;
    }
    
    return result;
}

// Convert string to 64 bit integer


long long StringToInt64(char *number, unsigned short base)
{
    long long result = 0LL;

    switch(base)
    {
       case 10:
       {
            char *pstr = number;
            unsigned int place_values = StringLength(pstr);

            while(place_values)
            {
                unsigned char data;
                
                if((*pstr <= '9') && (*pstr >= '0'))
                {
                    data = (unsigned char)(*pstr & 0xF);
                }
                else
                {
                    result = -1LL;
                    break;
                }
                
                unsigned long long power = 1LL;
                unsigned int i;

                for(i = power; i < place_values; i++)
                {
                    power *= base;
                }
             
                result += (long long)(data * power);
      
                place_values--;
                pstr++;
            }
        } 
        
        break; 

        case 16:
        {
            char *pstr = number;
            
            if((*pstr == '0') && (*(pstr + 1) == 'x'))
            {
                pstr += 2;
            }
            
            unsigned int place_values = StringLength(pstr);

            while(place_values)
            {   
                unsigned char data;
                
                if((*pstr <= 'f') && (*pstr >= 'A'))
                {
                    data = (unsigned char)((*pstr | 0x20) - 0x57);
                }
                else if((*pstr <= '9') && (*pstr >= '0'))
                {
                    data = (unsigned char)(*pstr & 0xF);
                }
                else
                {
                    result = -1LL;
                    break;
                }
                
                unsigned long long power = 1LL;
                unsigned int i;

                for(i = power; i < place_values; i++)
                {
                    power *= base;
                }
                
                result += (long long)(data * power);

                place_values--;
                pstr++;
            }
        }
        
        break;

        default:
            result = -1LL;
        break;
    }

    return result;
}


// Resets all previosly filtered addresses.


void ResetScan(MEMORY_BLOCK *mblock, BOOL reset_pid, BOOL disable_process_monitor)
{
    MEMORY_BLOCK *mb = mblock;

    EnableWindow(Scan, !reset_pid);

    SelectedProcessOpen = FALSE;
    addresses_frozen = 0;
    SelectedItem = -1;

    memset(frozen_addresses, 0, sizeof(frozen_addresses));
    memset(frozen_values, 0, sizeof(frozen_values));

    FirstScanNotRun = TRUE;
    SelectedItem = -1;
    unsigned short i;

    while(mb)
    {
        memset(mb->match_flag, 1, mb->size);
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
        EnableWindow(ChoosePid, TRUE);
    }

    ListView_DeleteAllItems(ListView);

    for(i = 0; i < ARRAYSIZE(data_types); i++)
    {
        SendMessage(DataSize, CB_ADDSTRING, 0, (LPARAM)data_types[i]);
    }

    TerminateThread(FreezeThread, 0);
    CloseHandle(FreezeThread);

    if(disable_process_monitor)
    {
        TerminateThread(MonitorSelectedProcessThread, 0);
        CloseHandle(MonitorSelectedProcessThread);
    }

    TerminateThread(ScanThread, 0);
    CloseHandle(ScanThread);
}

// Calls ResetScan() if the selected thread terminates.

void WINAPI MonitorSelectedProcess(void)
{
    while(SelectedProcessOpen)
    {
        DWORD code = -1;
        SelectedProcessOpen = ((GetExitCodeProcess(scanner->process, &code)) && (code == STILL_ACTIVE));

        if(!SelectedProcessOpen && scanner)
        {
            ResetScan(scanner, TRUE, TRUE);
            break;
        }
    }
}

// Checks if a string is a decimal.


BOOL IsNumeric(char *str)
{
    BOOL numeric = (str) ? TRUE : FALSE;
    char *tmpstr = str;
    unsigned int index = 0;

    while((*tmpstr) && (numeric))
    {
        numeric = (((*tmpstr >= 48) && (*tmpstr <= 57)) ||
                   ((index == 0) && (*tmpstr == 45)) ||
                   ((*tmpstr == 46)));

        if(!numeric) break;

        index++;
        tmpstr++;
    }

    return numeric;
}

// Finds the number of matches from the last scan.

unsigned int GetMatchCount(MEMORY_BLOCK *mblock)
{
    unsigned int matches = 0;
    MEMORY_BLOCK *mb = mblock;

    while(mb)
    {
        matches += mb->matches;
        mb = mb->next;
    }

    return matches;
}


// Finds all running processes on machine and exListViewContextMenucts the process id.


BOOL GetProcessNameAndID(void)
{
    HANDLE snapshot, process;
    PROCESSENTRY32 pe;

    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if(snapshot == INVALID_HANDLE_VALUE) return FALSE;

    pe.dwSize = sizeof(PROCESSENTRY32);

    if(!Process32First(snapshot, &pe))
    {
        CloseHandle(snapshot);
        return FALSE;
    }

    do
    {
        if(pe.th32ProcessID == GetCurrentProcessId()) continue;

        _snprintf(processes[number_of_processes], sizeof(processes[number_of_processes]), "%s", pe.szExeFile);

        process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID); 

        if(process)
        {
            _snprintf(pids[number_of_processes], sizeof(pids[number_of_processes]), "%u", pe.th32ProcessID);
            number_of_processes++; 

            CloseHandle(process);
        }

    } while(Process32Next(snapshot, &pe));

    process_count = number_of_processes;
    number_of_processes = 0;

    CloseHandle(snapshot);

    return TRUE;
}


// Populates the ListView with the results of the scan.


void AddItemToListView(char *address, char *val)
{
    memset(&Item, 0, sizeof(Item));

    Item.mask        = LVIF_TEXT;
    Item.iSubItem    = 0;
    Item.pszText     = address;

    SendMessage(ListView, LVM_INSERTITEM, 0, (LPARAM)&Item);

    Item.iSubItem    = 1;
    Item.pszText     = val;

    SendMessage(ListView, LVM_SETITEM, 0, (LPARAM)&Item);
}


// A set of helper functions that reads/writes the memory at the address specified.


float PeekFloat(HANDLE process, unsigned char *address, unsigned short data_size)
{
    float ret = 0;
    if(ReadProcessMemory(process, address, &ret, data_size, 0))
        return ret;
    return 0;
}

double PeekDouble(HANDLE process, unsigned char *address, unsigned short data_size)
{
    double ret = 0;
    if(ReadProcessMemory(process, address, &ret, data_size, 0))
        return ret;
    return 0;
}

long long PeekDecimal(HANDLE process, unsigned char *address, unsigned short data_size)
{
    long long ret = 0;
    if(ReadProcessMemory(process, address, &ret, data_size, 0))
        return ret;
    return 0;
}

BOOL PokeFloat(HANDLE process, unsigned long long address, float val, unsigned short data_size)
{
    unsigned long long bytes_read = 0;
    if (WriteProcessMemory(process, (LPVOID)address, (LPCVOID)&val, data_size, (SIZE_T *)&bytes_read) != 0)
        return (bytes_read == data_size);
    return FALSE;
}

BOOL PokeDouble(HANDLE process, unsigned long long address, double val, unsigned short data_size)
{
    unsigned long long bytes_read = 0;
    if (WriteProcessMemory(process, (LPVOID)address, (LPCVOID)&val, data_size, (SIZE_T *)&bytes_read) != 0)
        return (bytes_read == data_size);
    return FALSE;
}

BOOL PokeDecimal(HANDLE process, unsigned long long address, long long val, unsigned short data_size)
{
    unsigned long long bytes_read = 0;
    if (WriteProcessMemory(process, (LPVOID)address, (LPCVOID)&val, data_size, (SIZE_T *)&bytes_read) != 0)
        return (bytes_read == data_size);
    return FALSE;
}


// Constructs and allocates the MEMORY_BLOCK linked list structure.


MEMORY_BLOCK *CreateMemoryBlock(HANDLE process, MEMORY_BASIC_INFORMATION *mbi, unsigned short data_size)
{
    MEMORY_BLOCK *mb = (MEMORY_BLOCK *)malloc(sizeof(*mb));

    if(mb)
    {
        mb->process             = process;
        mb->size                = mbi->RegionSize;
        mb->data_size           = data_size;
        mb->address             = (unsigned char *)mbi->BaseAddress;
        mb->buffer              = (unsigned char *)malloc(mbi->RegionSize);
        mb->match_flag          = (unsigned char *)malloc(mbi->RegionSize);
        mb->matches             = mb->size;
        mb->next                = 0;

        if(mb->match_flag) memset(mb->match_flag, 1, mb->size);
    }

    return mb;
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

    mb = 0;
}

// Finds the initial valid memory information and sets up for UpdateMemoryBlock().


MEMORY_BLOCK *CreateMemoryScanner(DWORD pid, unsigned short data_size)
{
    MEMORY_BLOCK *ret = 0;
    unsigned char *address = 0;
    MEMORY_BASIC_INFORMATION mbi;
    current_pid = pid;

    Process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if(Process)
    {
        SelectedProcessOpen = TRUE;

        while(VirtualQueryEx(Process, address, &mbi, sizeof(mbi)))
        {
            if((mbi.State & MEM_COMMIT) && (mbi.Protect & MEM_WRITABLE))
            {
                MEMORY_BLOCK *mb = CreateMemoryBlock(Process, &mbi, data_size);

                if(mb)
                {
                    mb->next = ret;
                    ret = mb;
                }
            }

            address = (unsigned char *)mbi.BaseAddress + mbi.RegionSize;
        }
    }
    else
    {
        EnableWindow(ChoosePid, FALSE);

        ResetScan(0, TRUE, TRUE);
        EnableWindow(MainWindow, FALSE);

        MessageBox(0, "Error: Failed to open process.", title, MB_OK);

        EnableWindow(MainWindow, TRUE);
        EnableWindow(ChoosePid, TRUE);

        SetForegroundWindow(MainWindow);
    }
    return ret;
}

BOOL SelectedAddressFrozen(void)
{
    BOOL frozen = FALSE;
    unsigned int i;

    char address[256];

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address));

    for(i = 0; i < addresses_frozen; i++)
    {
        if(strcmp(frozen_addresses[i], address) == 0)
        {
            frozen = TRUE;
            break;
        }
    }

    return frozen;
}

// A thread to monitor addresses for change.

void WINAPI FreezeAddresses(void)
{
    for(;;)
    {
        if(addresses_frozen)
        {
            unsigned int i;

            for(i = 0; i < addresses_frozen; i++)
            {
                double value = strtod(frozen_values[i], 0);
                unsigned long long address = StringToInt64(frozen_addresses[i], 16);

                switch(type)
                {
                    case TYPE_FLOAT:
                        current_value = PeekFloat(scanner->process, (unsigned char *)address, scanner->data_size);
                        if(value != current_value) PokeFloat(scanner->process, address, (float)value, scanner->data_size);
                    break;

                    case TYPE_DOUBLE:
                        current_value = PeekDouble(scanner->process, (unsigned char *)address, scanner->data_size);
                        if(value != current_value) PokeDouble(scanner->process, address, value, scanner->data_size);
                    break;

                    case TYPE_DECIMAL:
                        current_value = PeekDecimal(scanner->process, (unsigned char *)address, scanner->data_size);
                        if(value != current_value) PokeDecimal(scanner->process, address, (unsigned long long)value, scanner->data_size);
                    break;
                }
            }
        }
    }
}


// Filters memory information aquired by CreateMemoryScanner() and subsequent calls to this function.


void UpdateMemoryBlock(MEMORY_BLOCK *mblock, SEARCH_CONDITION condition, TYPE type, double val)
{
    MEMORY_BLOCK *mb = mblock;
    SelectedItem = -1;

    while(mb)
    {
        char data_size[256];
        static unsigned char buffer[128 * 1024];
        static unsigned long long total_read, bytes_left, bytes_to_read, bytes_read;

        bytes_left = mb->size;
        total_read = 0;
        mb->matches = 0;

        int selection_id = (int)SendMessage(DataSize, CB_GETCURSEL, 0, 0);
        _snprintf(data_size, sizeof(data_size), "%s", data_sizes[selection_id]);
        mb->data_size = (unsigned short)atoi(data_size);

        while(bytes_left)
        {
            bytes_to_read = (bytes_left > sizeof(buffer)) ? sizeof(buffer) : bytes_left;

            ReadProcessMemory(mb->process, mb->address + total_read, buffer, bytes_to_read, (SIZE_T *)&bytes_read);
            if(bytes_read != bytes_to_read) break;

            double tmpval, prevval;
            unsigned int offset;
            BOOL match;

            for(offset = 0; offset < bytes_read; offset += mb->data_size)
            {
                if(mb->match_flag[total_read + offset])
                {
                    if(type == TYPE_DECIMAL)
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
                        ++mb->matches;
                    else
                        mb->match_flag[total_read + offset] = 0;
                }
            }

            memcpy(mb->buffer + total_read, buffer, bytes_read);

            bytes_left -= bytes_read;
            total_read += bytes_read;
        }

        mb->size = total_read;
        mb = mb->next;

    }
}


// Add scan results to user interface.


void DisplayScanResults(MEMORY_BLOCK *mblock)
{
    MEMORY_BLOCK *mb = mblock;

    while(mb)
    {
        unsigned int offset;

        for(offset = 0; offset < mb->size; offset += mb->data_size)
        {
            if(mb->match_flag[offset])
            {
                char address[256], val[256];

                _snprintf(address, sizeof(address), "%p", mb->address + offset);

                if(type == TYPE_DECIMAL)
                {
                    long long value = PeekDecimal(mb->process, mb->address + offset, mb->data_size);
                    _snprintf(val, sizeof(val), "%lld", value);
                }

                else if(type == TYPE_FLOAT)
                {
                    float value = PeekFloat(mb->process, mb->address + offset, mb->data_size);
                    _snprintf(val, sizeof(val), "%f", value);
                }

                else if(type == TYPE_DOUBLE)
                {
                    double value = PeekDouble(mb->process, mb->address + offset, mb->data_size);
                    _snprintf(val, sizeof(val), "%f", value);
                }

                AddItemToListView(address, val);
            }
        }
        mb = mb->next;
    }
}


// The thread function responsible for performing the scan.


void WINAPI ProcessScan(void)
{
    unsigned int matches;
    char pid[256], data_size[256], val[256], condition[256], message[256];

    SendMessage(Pid, WM_GETTEXT, sizeof(pid), (LPARAM)pid);
    SendMessage(Value, WM_GETTEXT, sizeof(val), (LPARAM)val);
    SendMessage(SearchCondition, WM_GETTEXT, sizeof(condition), (LPARAM)condition);

    int selection_id = (int)SendMessage(DataSize, CB_GETCURSEL, 0, 0);
    if(selection_id > -1) _snprintf(data_size, sizeof(data_size), "%s", data_sizes[selection_id]);

    if(!IsNumeric(val))
    {
        char msg[] = "Search value must be in decimal format";
        MessageBox(0, msg, title, MB_OK);
    }

    else if((StringLength(pid) && StringLength(data_size) && StringLength(val)) && (lstrcmp(pid, "*No Process Selected*") != 0))
    {
        scanner = (scanner) ? scanner : CreateMemoryScanner((unsigned int)StringToInt64(pid, 10), (unsigned short)StringToInt64(data_size, 10)); 

        if(scanner)
        {
            TerminateThread(MonitorSelectedProcessThread, 0);
            CloseHandle(MonitorSelectedProcessThread);

            MonitorSelectedProcessThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MonitorSelectedProcess, 0, 0, 0);

            if(MonitorSelectedProcessThread)
            {
                SendMessage(ListView, LVM_DELETEALLITEMS, 0, 0);
                TerminateThread(FreezeThread, 0);
                CloseHandle(FreezeThread);

                addresses_frozen = 0;
                ScanRunning = TRUE;

                int selected_search_condition = (int)SendMessage(SearchCondition, CB_GETCURSEL, 0, 0);

                if(selected_search_condition > -1)
                {
                    EnableWindow(ChangeValue, FALSE);
                    EnableWindow(Scan, FALSE);
                    EnableWindow(NewScan, FALSE);
                    EnableWindow(ChoosePid, FALSE);

                    type = (selection_id <= 3) ? TYPE_DECIMAL : (selection_id == 5) ? TYPE_DOUBLE : TYPE_FLOAT;

                    if(type == TYPE_DECIMAL)
                    {
                        switch(selected_search_condition)
                        {
                            case SEARCH_EQUALS:
                                UpdateMemoryBlock(scanner, SEARCH_EQUALS, TYPE_DECIMAL, StringToInt64(val, 10));
                                DisplayScanResults(scanner);
                            break;

                            case SEARCH_INCREASED:
                                UpdateMemoryBlock(scanner, SEARCH_INCREASED, TYPE_DECIMAL, 0);
                                DisplayScanResults(scanner);
                            break;

                            case SEARCH_DECREASED:
                                UpdateMemoryBlock(scanner, SEARCH_DECREASED, TYPE_DECIMAL, 0);
                                DisplayScanResults(scanner);
                            break;
                        }
                    }

                    else if(type == TYPE_FLOAT)
                    {
                        switch(selected_search_condition)
                        {
                            case SEARCH_EQUALS:
                                UpdateMemoryBlock(scanner, SEARCH_EQUALS, TYPE_FLOAT, atof(val));
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
                                UpdateMemoryBlock(scanner, SEARCH_EQUALS, TYPE_DOUBLE, atof(val));
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
                        TerminateThread(FreezeThread, 0);
                        CloseHandle(FreezeThread);
                        FreezeThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)FreezeAddresses, 0, 0, 0);
                    }

                    if(matches > 1 || matches == 0)
                        _snprintf(message, sizeof(message), "Scan finished! %u matches found.", matches);
                    else
                        _snprintf(message, sizeof(message), "Scan finished! %u match found.", matches);


                    if(FirstScanNotRun)
                    {
                        SendMessage(SearchCondition, CB_ADDSTRING, 0, (LPARAM)search_conditions[SEARCH_INCREASED]);
                        SendMessage(SearchCondition, CB_ADDSTRING, 0, (LPARAM)search_conditions[SEARCH_DECREASED]);

                        FirstScanNotRun = FALSE;
                    }

                    ScanRunning = FALSE;

                    EnableWindow(Scan, TRUE);
                    EnableWindow(NewScan, TRUE);
                    EnableWindow(ChangeValue, TRUE);
                    EnableWindow(ChoosePid, TRUE);

                    EnableWindow(MainWindow, FALSE);
                    MessageBox(0, message, title, MB_OK);
                    EnableWindow(MainWindow, TRUE);

                    SetForegroundWindow(MainWindow);

                }
            }
        }
    }
}

// Once an address is found, this function updates the value at that address.


BOOL UpdateValue(void)
{
    char address[256], val[256], buffer[256];

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address));
    SendMessage(ChangeValueDlgValue, WM_GETTEXT, sizeof(val), (LPARAM)val);

    unsigned long long addr = StringToInt64(address, 16);
    double v = (double)atof(val);

    LVITEM Item;
    memset(&Item, 0, sizeof(Item));

    Item.mask = LVIF_TEXT;
    Item.iItem = SelectedItem;
    Item.iSubItem = 1;

    memset(buffer, 0, sizeof(buffer));

    SendMessage(Value, WM_GETTEXT, sizeof(buffer), (LPARAM)buffer);
    Item.pszText = buffer;
    ListView_SetItem(ListView, &Item);

    if(type == TYPE_FLOAT)
    {
        if(PokeFloat(scanner->process, addr, v, scanner->data_size))
        {
            DestroyWindow(ChangeValueDlg);
            EnableWindow(MainWindow, FALSE);
            MessageBox(0, "Memory was updated successfully.", "Success!", MB_OK);
            EnableWindow(MainWindow, TRUE);
            SetForegroundWindow(MainWindow);
            SendMessage(ChangeValueDlgValue, WM_GETTEXT, sizeof(val), (LPARAM)val);
            float tmp = PeekFloat(scanner->process, (unsigned char *)addr, scanner->data_size);
            _snprintf(val, sizeof(val), "%f", tmp);
            ListView_SetItemText(ListView, SelectedItem, 1, val);

            return TRUE;
        }
        else
        {
            char error[255];
            memset(error, 0, sizeof(error));
            _snprintf(error, sizeof(error), "Memory operation failed! (%d)", GetLastError());
            EnableWindow(MainWindow, FALSE);
            MessageBox(0, error, "Error!", MB_OK);
            EnableWindow(MainWindow, TRUE);
            SetForegroundWindow(MainWindow);

            return FALSE;
        }
    }

    else if(type == TYPE_DOUBLE)
    {
        if(PokeDouble(scanner->process, addr, v, scanner->data_size))
        {
            DestroyWindow(ChangeValueDlg);
            EnableWindow(MainWindow, FALSE);
            MessageBox(0, "Memory was updated successfully.", "Success!", MB_OK);
            EnableWindow(MainWindow, TRUE);
            SetForegroundWindow(MainWindow);
            SendMessage(ChangeValueDlgValue, WM_GETTEXT, sizeof(val), (LPARAM)val);
            double tmp = PeekDouble(scanner->process, (unsigned char *)addr, scanner->data_size);
            _snprintf(val, sizeof(val), "%f", tmp);
            ListView_SetItemText(ListView, SelectedItem, 1, val);

            return TRUE;
        }
        else
        {
            char error[255];
            memset(error, 0, sizeof(error));
            _snprintf(error, sizeof(error), "Memory operation failed! (%d)", GetLastError());
            EnableWindow(MainWindow, FALSE);
            MessageBox(0, error, "Error!", MB_OK);
            EnableWindow(MainWindow, TRUE);
            SetForegroundWindow(MainWindow);

            return FALSE;
        }
    }

    else if(type == TYPE_DECIMAL)
    {
        if(PokeDecimal(scanner->process, addr, (long long)v, scanner->data_size))
        {
            DestroyWindow(ChangeValueDlg);
            EnableWindow(MainWindow, FALSE);
            MessageBox(0, "Memory was updated successfully.", "Success!", MB_OK);
            EnableWindow(MainWindow, TRUE);
            SetForegroundWindow(MainWindow);

            SendMessage(ChangeValueDlgValue, WM_GETTEXT, sizeof(val), (LPARAM)val);
            long long tmp = PeekDecimal(scanner->process, (unsigned char *)addr, scanner->data_size);
            _snprintf(val, sizeof(val), "%lld", tmp);
            ListView_SetItemText(ListView, SelectedItem, 1, val);

            return TRUE;
        }
        else
        {
            char error[255];
            memset(error, 0, sizeof(error));
            _snprintf(error, sizeof(error), "Memory operation failed! (%d)", GetLastError());
            EnableWindow(MainWindow, FALSE);
            MessageBox(0, error, "Error!", MB_OK);
            EnableWindow(MainWindow, TRUE);
            SetForegroundWindow(MainWindow);

            return FALSE;
        }
    }

    return FALSE;
}

#endif
