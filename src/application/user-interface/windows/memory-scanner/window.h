#ifndef _MEMORY_SCANNER_WINDOW_H
#define _MEMORY_SCANNER_WINDOW_H

HWND CreateMemoryScannerWindow(VOID)
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
                                              CW_USEDEFAULT, CW_USEDEFAULT, 625, 405, null, null, Instance, null);

        if(MemoryScannerWindow)
        {
            NONCLIENTMETRICSA metrics;
            uint8 data_types_list_index;
            LVCOLUMNA address_column;
            LVCOLUMNA value_column;
            
            data_types_list_index = 0;

            metrics.cbSize = sizeof(metrics);
            SystemParametersInfoA(SPI_GETNONCLIENTMETRICS, sizeof(metrics), &metrics, 0);

            Font = CreateFontIndirectA(&metrics.lfMessageFont);

            ListView = CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTVIEW, null, 
                                       WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SINGLESEL,
                                       10, 10, 598, 225, MemoryScannerWindow, (HMENU)ID_LISTVIEW, Instance, null);

            SearchConditionLabel = CreateWindowA("static", "Search Condition: ", WS_VISIBLE | WS_CHILD, 
                                                 10, 245, 100, 25, MemoryScannerWindow, null, 
                                                 Instance, null);

            SearchCondition = CreateWindowA("combobox", null, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 
                                            150, 245, 125, 200, MemoryScannerWindow, 
                                            (HMENU)ID_SEARCH_CONDITION, 
                                            Instance, null);

            ValueLabel = CreateWindowA("static", "Search Value: ", WS_VISIBLE | WS_CHILD, 
                                       310, 250, 100, 25, MemoryScannerWindow, 
                                       null, Instance, null);

            Value = CreateWindowA("edit", null, WS_VISIBLE | WS_CHILD | WS_BORDER, 
                                  400, 247, 100, 20, MemoryScannerWindow, (HMENU)ID_VALUE, 
                                  Instance, null);

            ChangeValue = CreateWindowA("button", "Change Value", WS_VISIBLE | WS_CHILD, 
                                        510, 245, 100, 25, MemoryScannerWindow,
                                        (HMENU)ID_CHANGE_VALUE, 
                                        Instance, null);

            NewScan = CreateWindowA("button", "Reset Scan", WS_VISIBLE | WS_CHILD, 
                                    510, 275, 100, 25, MemoryScannerWindow, 
                                    (HMENU)ID_NEW_SCAN, 
                                    Instance, null);

            Pid = CreateWindowA("static", "*No Process Selected*", WS_VISIBLE | WS_CHILD, 
                                10, 280, 150, 25, MemoryScannerWindow, 
                                (HMENU)ID_PROCESS_ID, 
                                Instance, null);

            ChoosePid = CreateWindowA("button", "Select Process", WS_VISIBLE | WS_CHILD, 
                                      175, 275, 100, 25, MemoryScannerWindow, 
                                      (HMENU)ID_SELECT_PROCESS, 
                                      Instance, null);

            DataSizeLabel = CreateWindowA("static", "Type: ", WS_VISIBLE | WS_CHILD, 
                                          310, 280, 100, 25, MemoryScannerWindow, null, 
                                          Instance, null);

            DataSize = CreateWindowA("combobox", null, WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_DROPDOWNLIST, 
                                     400, 275, 100, 200, MemoryScannerWindow, (HMENU)ID_VALUE, 
                                     Instance, null);

            Scan = CreateWindowA("button", "Scan Memory", WS_VISIBLE | WS_CHILD, 
                                 10, 315, 600, 50, MemoryScannerWindow, (HMENU)ID_SCAN, 
                                 Instance, null);

            while(data_types_list_index < (uint8)ARRAYSIZE(Datatypes))
            {
                SendMessageA(DataSize, CB_ADDSTRING, 0, (LPARAM)Datatypes[data_types_list_index]);
                ++data_types_list_index;
            }

            EnableWindow(Scan, false);
            EnableWindow(NewScan, false);
            EnableWindow(ChangeValue, false);
            EnableWindow(DataSize, false);
            EnableWindow(SearchCondition, false);
            EnableWindow(ListView, false);
            EnableWindow(Value, false);

            SendMessageA(ListView, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(ChangeValue, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(ChoosePid, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(NewScan, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(Scan, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(DataSizeLabel, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(ValueLabel, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(DataSize, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(Pid, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(Value, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(SearchCondition, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(SearchConditionLabel, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));

            UpdateLayoutForDpi(MemoryScannerWindow, CW_USEDEFAULT, CW_USEDEFAULT, 625, 405);
            UpdateLayoutForDpi(ListView, 10, 10, 598, 225);
            UpdateLayoutForDpi(SearchCondition, 150, 245, 125, 200);
            UpdateLayoutForDpi(SearchConditionLabel, 10, 245, 100, 25);
            UpdateLayoutForDpi(ValueLabel, 310, 250, 100, 25);
            UpdateLayoutForDpi(Value, 400, 247, 100, 20);
            UpdateLayoutForDpi(ChangeValue, 510, 245, 100, 25);
            UpdateLayoutForDpi(NewScan, 510, 275, 100, 25);
            UpdateLayoutForDpi(Pid, 10, 280, 150, 25);
            UpdateLayoutForDpi(ChoosePid, 175, 275, 100, 25);
            UpdateLayoutForDpi(DataSizeLabel, 310, 280, 100, 25);
            UpdateLayoutForDpi(DataSize, 400, 275, 100, 200);
            UpdateLayoutForDpi(Scan, 10, 315, 600, 50);

            address_column.cx = MulDiv(288, ScreenDPI, 96); 
            value_column.cx = MulDiv(288, ScreenDPI, 96);

            address_column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
            address_column.pszText = (string)"Address";
            address_column.iSubItem = 0;

            value_column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
            value_column.pszText = (string)"Value";
            value_column.iSubItem = 1;

            SendMessageA(ListView, LVM_INSERTCOLUMN, 0, (LPARAM)&address_column);
            SendMessageA(ListView, LVM_INSERTCOLUMN, 1, (LPARAM)&value_column);
            SendMessageA(SearchCondition, CB_ADDSTRING, 0, (LPARAM)SearchConditions[SEARCH_EQUALS]);
            SendMessageA(ListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES);

            CenterWindow(MemoryScannerWindow);
            ShowWindow(MemoryScannerWindow, SW_SHOW);
            UpdateWindow(MemoryScannerWindow);

            ShowScrollBar(ListView, SB_HORZ, false);
            ShowScrollBar(ListView, SB_VERT, true);

            return MemoryScannerWindow;
        }
    }

    return null;
}

#endif
