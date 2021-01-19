#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

int32 CreateMainWindow(void)
{
    uint8 data_types_list_index;
    LVCOLUMNA AddressColumn, ValueColumn;
    NONCLIENTMETRICSA metrics;
    WNDCLASSEXA wc;
    MSG Msg;

    data_types_list_index = 0;
    Instance = GetModuleHandleA(null);

    wc.cbSize = sizeof(wc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.hCursor = LoadCursorA(Instance, IDC_ARROW);
    wc.hIcon = LoadIconA(Instance, MAKEINTRESOURCE(AppIcon));
    wc.hIconSm = LoadIconA(Instance, MAKEINTRESOURCE(AppIcon));
    wc.hInstance = Instance;
    wc.lpfnWndProc = MainWindowProc;
    wc.lpszClassName = "Main";
    wc.lpszMenuName = null;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    UnregisterClassA(wc.lpszClassName, Instance);

    if(RegisterClassExA(&wc))
    {
        MainWindow = CreateWindowExA(WS_EX_STATICEDGE, wc.lpszClassName, title,
                                     WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, 
                                     100, 100, Width, Height, null, 
                                     null, Instance, null);

        if(MainWindow)
        {
            metrics.cbSize = sizeof(metrics);
            SystemParametersInfoA(SPI_GETNONCLIENTMETRICS, sizeof(metrics), &metrics, 0);
            Font = CreateFontIndirectA(&metrics.lfMessageFont);

            ListView = CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTVIEW, null, 
                                       WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SINGLESEL,
                                       10, 10, 598, 225, MainWindow, (HMENU)ID_LISTVIEW, Instance, null);

            AddressColumn.cx = 298;
            AddressColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
            AddressColumn.pszText = "Address";
            AddressColumn.iSubItem = 0;

            ValueColumn.cx = 298;
            ValueColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
            ValueColumn.pszText = "Value";
            ValueColumn.iSubItem = 1;

            SearchConditionLabel = CreateWindowA("static", "Search Condition: ", WS_VISIBLE | WS_CHILD, 
                                                 10, 245, 100, 25, MainWindow, null, 
                                                 Instance, null);

            SearchCondition = CreateWindowA("combobox", null, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 
                                            150, 245, 125, 200, MainWindow, 
                                            (HMENU)ID_SEARCH_CONDITION, 
                                            Instance, null);

            ValueLabel = CreateWindowA("static", "Value: ", WS_VISIBLE | WS_CHILD, 
                                       310, 250, 100, 25, MainWindow, 
                                       null, Instance, null);

            Value = CreateWindowExA(WS_EX_CLIENTEDGE, "edit", null, WS_VISIBLE | WS_CHILD, 
                                    400, 247, 100, 20, MainWindow, (HMENU)ID_VALUE, 
                                    Instance, null);

            ChangeValue = CreateWindowA("button", "Change Value", WS_VISIBLE | WS_CHILD, 
                                        510, 245, 100, 25, MainWindow,
                                        (HMENU)ID_CHANGE_VALUE, 
                                        Instance, null);

            NewScan = CreateWindowA("button", "Reset Scan", WS_VISIBLE | WS_CHILD, 
                                    510, 275, 100, 25, MainWindow, 
                                    (HMENU)ID_NEW_SCAN, 
                                    Instance, null);

            Pid = CreateWindowA("static", "*No Process Selected*", WS_VISIBLE | WS_CHILD, 
                                10, 280, 150, 25, MainWindow, 
                                (HMENU)ID_PROCESS_ID, 
                                Instance, null);

            ChoosePid = CreateWindowA("button", "Select Process", WS_VISIBLE | WS_CHILD, 
                                      175, 275, 100, 25, MainWindow, 
                                      (HMENU)ID_SELECT_PROCESS, 
                                      Instance, null);

            DataSizeLabel = CreateWindowA("static", "Type: ", WS_VISIBLE | WS_CHILD, 
                                          310, 280, 100, 25, MainWindow, null, 
                                          Instance, null);

            DataSize = CreateWindowA("combobox", null, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 
                                     400, 275, 100, 200, MainWindow, (HMENU)ID_VALUE, 
                                     Instance, null);

            Scan = CreateWindowA("button", "Scan Memory", WS_VISIBLE | WS_CHILD, 
                                 10, 315, 600, 50, MainWindow, (HMENU)ID_SCAN, 
                                 Instance, null);

            EnableWindow(Scan, false);
            EnableWindow(NewScan, false);
            EnableWindow(ChangeValue, false);

            while(data_types_list_index < (uint8)ARRAYSIZE(data_types))
            {
                SendMessageA(DataSize, CB_ADDSTRING, 0, (LPARAM)data_types[data_types_list_index]);
                data_types_list_index++;
            }

            SendMessageA(ListView, LVM_INSERTCOLUMN, 0, (LPARAM)&AddressColumn);
            SendMessageA(ListView, LVM_INSERTCOLUMN, 1, (LPARAM)&ValueColumn);
            SendMessageA(SearchCondition, CB_ADDSTRING, 0, (LPARAM)search_conditions[SEARCH_EQUALS]);
            SendMessageA(ListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

            SendMessageA(ChangeValue, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(ChoosePid, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(NewScan, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(Scan, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(DataSizeLabel, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(PidLabel, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(ValueLabel, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(DataSize, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(Pid, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(Value, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(SearchCondition, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(SearchConditionLabel, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));

            ShowWindow(MainWindow, SW_SHOW);
            UpdateWindow(MainWindow);

            while(GetMessage(&Msg, null, 0, 0) > 0)
            {
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
            }

            return (int32)Msg.wParam;
        }
    }

    return -1;
}

#endif
