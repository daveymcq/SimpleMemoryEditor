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
                                       0, 0, 0, 0, MemoryScannerWindow, (HMENU)ID_LISTVIEW, Instance, null);

            SearchConditionLabel = CreateWindowA("static", "Condition: ", WS_VISIBLE | WS_CHILD, 
                                                 0, 0, 0, 0, MemoryScannerWindow, null, 
                                                 Instance, null);

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

            DataSizeLabel = CreateWindowA("static", "Size: ", WS_VISIBLE | WS_CHILD, 
                                          0, 0, 0, 0, MemoryScannerWindow, null, 
                                          Instance, null);

            DataSize = CreateWindowA("combobox", null, WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_DROPDOWNLIST, 
                                     0, 0, 0, 0, MemoryScannerWindow, (HMENU)ID_VALUE, 
                                     Instance, null);

            Scan = CreateWindowA("button", "Scan Memory", WS_VISIBLE | WS_CHILD, 
                                 0, 0, 0, 0, MemoryScannerWindow, (HMENU)ID_SCAN, 
                                 Instance, null);

            while(data_types_list_index < (uint8)ARRAYSIZE(Datatypes))
            {
                SendMessageA(DataSize, CB_ADDSTRING, 0, (LPARAM)Datatypes[data_types_list_index]);
                ++data_types_list_index;
            }

            EnableWindow(Scan, false);
            EnableWindow(Value, false);
            EnableWindow(NewScan, false);
            EnableWindow(ListView, false);
            EnableWindow(DataSize, false);
            EnableWindow(SearchCondition, false);

            SendMessageA(ListView, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(ChoosePid, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(NewScan, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(Scan, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(DataSizeLabel, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(DataSize, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(Value, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(SearchCondition, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(SearchConditionLabel, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));

            UpdateWindowForDpi(MemoryScannerWindow, CW_USEDEFAULT, CW_USEDEFAULT, 625, 405);
            UpdateWindowForDpi(ChoosePid, 10, 244, 100, 25);
            UpdateWindowForDpi(ListView, 10, 10, 598, 225);
            UpdateWindowForDpi(SearchCondition, 75, 275, 135, 20);
            UpdateWindowForDpi(SearchConditionLabel, 10, 276, 54, 25);
            UpdateWindowForDpi(Value, 120, 245, 490, 23);
            UpdateWindowForDpi(NewScan, 400, 274, 210, 25);
            UpdateWindowForDpi(DataSizeLabel, 220, 276, 100, 25);
            UpdateWindowForDpi(DataSize, 255, 275, 135, 20);
            UpdateWindowForDpi(Scan, 10, 305, 600, 60);

            address_column.cx = MulDiv(298, ScreenDPI, 96); 
            value_column.cx = MulDiv(298, ScreenDPI, 96);

            address_column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
            address_column.pszText = (string)"Address";
            address_column.iSubItem = 0;

            value_column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
            value_column.pszText = (string)"Value";
            value_column.iSubItem = 1;

            SendMessageA(ListView, LVM_INSERTCOLUMN, 0, (LPARAM)&address_column);
            SendMessageA(ListView, LVM_INSERTCOLUMN, 1, (LPARAM)&value_column);
            SendMessageA(SearchCondition, CB_ADDSTRING, 0, (LPARAM)SearchConditions[SEARCH_EQUALS]);
            SendMessageA(ListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

            ShowWindow(MemoryScannerWindow, SW_SHOW);
            UpdateWindow(MemoryScannerWindow);
            CenterWindow(MemoryScannerWindow);

            return MemoryScannerWindow;
        }
    }

    return null;
}

#endif
