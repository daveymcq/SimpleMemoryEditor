#ifndef _MEMORY_SCANNER_WINDOW_H
#define _MEMORY_SCANNER_WINDOW_H

HWND CreateMemoryScannerWindow(void)
{
    WNDCLASSEXA wc;

    wc.cbSize = sizeof(wc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.hCursor = LoadCursorA(Instance, IDC_ARROW);
    wc.hIcon = LoadIconA(Instance, MAKEINTRESOURCEA(AppIcon));
    wc.hIconSm = LoadIconA(Instance, MAKEINTRESOURCEA(AppIcon));
    wc.hInstance = Instance;
    wc.lpfnWndProc = (WNDPROC)MainWindowProc;
    wc.lpszClassName = (string)"MemoryScannerWindow";
    wc.lpszMenuName = null;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    UnregisterClassA(wc.lpszClassName, Instance);

    if(RegisterClassExA(&wc))
    {
        MemoryScannerWindow = CreateWindowExA(WS_EX_STATICEDGE, wc.lpszClassName, Title,
                                              WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, 
                                              0, 0, 0, 0, null, null, 
                                              Instance, null);
        if(MemoryScannerWindow)
        {
            uint16 data_types_list_index;
            NONCLIENTMETRICSA metrics;
            LVCOLUMNA address_column;
            LVCOLUMNA value_column;
             
            metrics.cbSize = sizeof(metrics);
            SystemParametersInfoA(SPI_GETNONCLIENTMETRICS, sizeof(metrics), &metrics, 0);

            Font = CreateFontIndirectA(&metrics.lfMessageFont);

            ListView = CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTVIEW, null, 
                                       WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SINGLESEL | LVS_NOSCROLL,
                                       CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, MemoryScannerWindow, (HMENU)ID_LISTVIEW, Instance, null);

            SearchCondition = CreateWindowA("combobox", null, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 
                                            0, 0, 0, 0, MemoryScannerWindow, 
                                            (HMENU)ID_SEARCH_CONDITION, 
                                            Instance, null);

            Value = CreateWindowA("edit", null, WS_VISIBLE | WS_CHILD | WS_BORDER, 
                                  0, 0, 0, 0, MemoryScannerWindow, (HMENU)ID_VALUE, 
                                  Instance, null);

            NewScan = CreateWindowA("button", "Reset Scan", WS_VISIBLE | WS_CHILD, 
                                    0, 0, 0, 0, MemoryScannerWindow, 
                                    (HMENU)ID_NEW_SCAN, 
                                    Instance, null);

            ChoosePid = CreateWindowA("button", "Select Process", WS_VISIBLE | WS_CHILD, 
                                      0, 0, 0, 0, MemoryScannerWindow, 
                                      (HMENU)ID_SELECT_PROCESS, 
                                      Instance, null);

            DataSize = CreateWindowA("combobox", null, WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_DROPDOWNLIST, 
                                     0, 0, 0, 0, MemoryScannerWindow, (HMENU)ID_VALUE, 
                                     Instance, null);

            Scan = CreateWindowA("button", "Scan Memory", WS_VISIBLE | WS_CHILD, 
                                 0, 0, 0, 0, MemoryScannerWindow, (HMENU)ID_SCAN, 
                                 Instance, null);

            ProgressBar = CreateWindowA(PROGRESS_CLASS, null, WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
                                        0, 0, 0, 0, MemoryScannerWindow, 
                                        (HMENU)ID_PROGRESS_BAR, 
                                        Instance, null);

            SetWindowLongPtr(ProgressBar, GWL_STYLE, GetWindowLongPtr(ProgressBar, GWL_STYLE) | PBS_SMOOTH);
            SendMessageA(ProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
            SendMessage(ProgressBar, PBM_SETMARQUEE, true, 0);
            SendMessageA(DataSize, CB_RESETCONTENT, 0, 0);
            SetTimer(MemoryScannerWindow, 1, 100, null); 
            SendMessageA(ProgressBar, PBM_SETPOS, 0, 0);

            for(data_types_list_index = 0; data_types_list_index < ARRAYSIZE(DataTypes); data_types_list_index++)
            {   
                SendMessageA(DataSize, CB_ADDSTRING, 0, (LPARAM)DataTypes[data_types_list_index]);
            }

            SendMessageA(ListView, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(ChoosePid, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(NewScan, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(Scan, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(DataSize, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(Value, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(SearchCondition, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));

            UpdateWindowForDpi(MemoryScannerWindow, CW_USEDEFAULT, CW_USEDEFAULT, 625, 410);
            UpdateWindowForDpi(ProgressBar, 400, 275, 210, 20);
            UpdateWindowForDpi(ChoosePid, 10, 245, 200, 22);
            UpdateWindowForDpi(ListView, 10, 10, 600, 225);
            UpdateWindowForDpi(SearchCondition, 10, 275, 200, 22);
            UpdateWindowForDpi(Value, 440, 245, 170, 20);
            UpdateWindowForDpi(NewScan, 220, 245, 210, 22);
            UpdateWindowForDpi(DataSize, 220, 275, 170, 20);
            UpdateWindowForDpi(Scan, 10, 305, 600, 60);

            address_column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
            address_column.cx = MulDiv(298, ScreenDPI, 96); 
            address_column.pszText = (string)"Address";
            address_column.fmt = LVCFMT_FIXED_WIDTH;
            address_column.iSubItem = 0;

            value_column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
            value_column.cx = MulDiv(298, ScreenDPI, 96);
            value_column.pszText = (string)"Value";
            value_column.fmt = LVCFMT_FIXED_WIDTH;
            value_column.iSubItem = 1;

            DisableListViewResizeThread = CreateThread(null, null, (LPTHREAD_START_ROUTINE)DisableListViewResize, null, null, null);

            if(DisableListViewResizeThread)
            {
                SendMessageA(ListView, LVM_INSERTCOLUMN, 0, (LPARAM)&address_column);
                SendMessageA(ListView, LVM_INSERTCOLUMN, 1, (LPARAM)&value_column);
                SendMessageA(SearchCondition, CB_ADDSTRING, 0, (LPARAM)SearchConditions[SEARCH_EQUALS]);
                SendMessageA(ListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES);

                SendMessageA(DataSize, CB_SETCURSEL, (WPARAM)2, 0);
                SendMessageA(SearchCondition, CB_SETCURSEL, (WPARAM)0, 0);
                
                EnableWindow(Scan, false);
                EnableWindow(Value, false);
                EnableWindow(NewScan, false);
                EnableWindow(DataSize, false);
                EnableWindow(ProgressBar, false);
                EnableWindow(SearchCondition, false);
                
                ShowWindow(MemoryScannerWindow, SW_SHOW);
                SetForegroundWindow(MemoryScannerWindow);
                CenterWindow(MemoryScannerWindow);
                UpdateWindow(MemoryScannerWindow);
                
                return MemoryScannerWindow;
            }
        }
    }

    return null;
}

#endif
