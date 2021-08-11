#ifndef _WINDOW_FUNCTIONS_H
#define _WINDOW_FUNCTIONS_H

/* The function function responsible for performing the scan. */

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

    if((val[0] == '0') && (val[1] == 'x'))
    {
        search_number_format = FMT_INT_HEXADECIMAL;
    }

    if((IsNumeric(val)) && ((StringLength(pid) && StringLength(data_size) && StringLength(val)) && (!StringCompare(pid, "*No Process Selected*", false))))
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

/* Center a window using the default display. */

void CenterWindow(HWND window)
{
    RECT window_rect;
    DWORD x, y, width, height;

    GetWindowRect(window, &window_rect);

    width = (window_rect.right - window_rect.left);
    height = (window_rect.bottom - window_rect.top);

    x =  ((GetSystemMetrics(SM_CXSCREEN) - width) / 2);
    y =  ((GetSystemMetrics(SM_CYSCREEN) - height) / 2);

    MoveWindow(window, x, y, width, height, true);
}

#endif
