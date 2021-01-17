#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

int32 CreateMainWindow(void)
{
    string value = "Value";
    string address = "Address";

    uint8 index;
    HINSTANCE hInstance;
    LVCOLUMN Column;
    NONCLIENTMETRICS metrics;
    WNDCLASSEX wc;
    MSG Msg;

    hInstance = GetModuleHandle(0);

    wc.cbSize = sizeof(wc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(AppIcon));
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(AppIcon));
    wc.hInstance = hInstance;
    wc.lpfnWndProc = MainWindowProc;
    wc.lpszClassName = "Main";
    wc.lpszMenuName = 0;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    UnregisterClass(wc.lpszClassName, 0);

    if(RegisterClassEx(&wc))
    {
        MainWindow = CreateWindowEx(WS_EX_STATICEDGE, wc.lpszClassName, title,
                                    WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, 
                                    100, 100, Width, Height, 0, 0, hInstance, 0);

        if(MainWindow)
        {
            metrics.cbSize = sizeof(metrics);
            SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(metrics), &metrics, 0);
            Font = CreateFontIndirect(&metrics.lfMessageFont);

            ListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, 0, 
                                      WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SINGLESEL,
                                      10, 10, 598, 225, MainWindow, (HMENU)ID_LISTVIEW, hInstance, 0);

            #ifndef LVS_EX_DOUBLEBUFFER
                #define LVS_EX_DOUBLEBUFFER 0x00010000
            #endif

            SendMessage(ListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

            Column.cx = 298;
            Column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
            Column.pszText = address;
            Column.iSubItem = 0;

            SendMessage(ListView, LVM_INSERTCOLUMN, 0, (LPARAM)&Column);

            Column.iSubItem = 1;
            Column.pszText = value;

            SendMessage(ListView, LVM_INSERTCOLUMN, 1, (LPARAM)&Column);

            SearchConditionLabel = CreateWindow("static", "Search Condition: ", WS_VISIBLE | WS_CHILD, 10, 245, 100, 25, MainWindow, 0, hInstance, 0);
            SearchCondition = CreateWindow("combobox", 0, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 150, 245, 125, 200, MainWindow, (HMENU)ID_SEARCH_CONDITION, hInstance, 0);

            SendMessage(SearchCondition, CB_ADDSTRING, 0, (LPARAM)search_conditions[SEARCH_EQUALS]);

            ValueLabel = CreateWindow("static", "Value: ", WS_VISIBLE | WS_CHILD, 310, 250, 100, 25, MainWindow, 0, hInstance, 0);
            Value = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", 0, WS_VISIBLE | WS_CHILD, 400, 247, 100, 20, MainWindow, (HMENU)ID_VALUE, hInstance, 0);
            ChangeValue = CreateWindow("button", "Change Value", WS_VISIBLE | WS_CHILD, 510, 245, 100, 25, MainWindow, (HMENU)ID_CHANGE_VALUE, hInstance, 0);
            NewScan = CreateWindow("button", "Reset Scan", WS_VISIBLE | WS_CHILD, 510, 275, 100, 25, MainWindow, (HMENU)ID_NEW_SCAN, hInstance, 0);

            Pid = CreateWindow("static", "*No Process Selected*", WS_VISIBLE | WS_CHILD, 10, 280, 150, 25, MainWindow, (HMENU)ID_PROCESS_ID, hInstance, 0);
            ChoosePid = CreateWindow("button", "Select Process", WS_VISIBLE | WS_CHILD, 175, 275, 100, 25, MainWindow, (HMENU)ID_SELECT_PROCESS, hInstance, 0);

            DataSizeLabel = CreateWindow("static", "Type: ", WS_VISIBLE | WS_CHILD, 310, 280, 100, 25, MainWindow, 0, GetModuleHandle(0), 0);
            DataSize = CreateWindow("combobox", 0, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 400, 275, 100, 200, MainWindow, (HMENU)ID_VALUE, hInstance, 0);

            for(index = 0; index < ARRAYSIZE(data_types); index++)
            {
                SendMessage(DataSize, CB_ADDSTRING, 0, (LPARAM)data_types[index]);
            }

            Scan = CreateWindow("button", "Scan Memory", WS_VISIBLE | WS_CHILD, 10, 315, 600, 50, MainWindow, (HMENU)ID_SCAN, hInstance, 0);

            EnableWindow(Scan, false);
            EnableWindow(NewScan, false);
            EnableWindow(ChangeValue, false);

            SendMessage(ChangeValue, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessage(ChoosePid, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessage(NewScan, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessage(Scan, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessage(DataSizeLabel, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessage(PidLabel, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessage(ValueLabel, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessage(DataSize, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessage(Pid, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessage(Value, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessage(SearchCondition, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessage(SearchConditionLabel, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));

            ShowWindow(MainWindow, SW_SHOW);
            UpdateWindow(MainWindow);

            while(GetMessage(&Msg, 0, 0, 0) > 0)
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
