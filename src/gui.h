#ifndef _GUI_H
#define _GUI_H

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SelectPidDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChangeValueDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

void CreateMainDialogUI(HWND hWnd)
{
    unsigned int i;
    char val_header[] = "Value";
    char addr_header[] = "Address";

    MenuBar = CreateMenu();
    FileMenu = CreateMenu();
    HelpMenu = CreateMenu();

    AppendMenu(FileMenu, MF_STRING, (UINT_PTR)ID_FILE_EXIT, "Exit");
    AppendMenu(HelpMenu, MF_STRING, (UINT_PTR)ID_HELP_ABOUT, "About");

    AppendMenu(MenuBar, MF_POPUP, (UINT_PTR)FileMenu, "File");
    AppendMenu(MenuBar, MF_POPUP, (UINT_PTR)HelpMenu, "Help");

    SetMenu(hWnd, MenuBar);

    metrics.cbSize = sizeof(NONCLIENTMETRICS);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &metrics, 0);
    font = CreateFontIndirect(&metrics.lfMessageFont);


    MemoryZero(&Column, sizeof(Column));

    ListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, 0, WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SINGLESEL,
                              10, 10, 598, 225, hWnd, (HMENU)ID_LISTVIEW, GetModuleHandle(0), 0);

    SendMessage(ListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);

    Column.cx = 298;
    Column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    Column.pszText = addr_header;
    Column.iSubItem = 0;

    SendMessage(ListView, LVM_INSERTCOLUMN, 0, (LPARAM)&Column);

    Column.iSubItem = 1;
    Column.pszText = val_header;

    SendMessage(ListView, LVM_INSERTCOLUMN, 1, (LPARAM)&Column);

    SearchConditionLabel = CreateWindow("static", "Search Condition: ", WS_VISIBLE | WS_CHILD, 10, 245, 100, 25, hWnd, 0, GetModuleHandle(0), 0);
    SearchCondition = CreateWindowEx(WS_EX_CLIENTEDGE, "combobox", 0, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 150, 245, 125, 25, hWnd, (HMENU)ID_SEARCH_CONDITION, GetModuleHandle(0), 0);

    SendMessage(SearchCondition, CB_ADDSTRING, 0, (LPARAM)search_conditions[SEARCH_EQUALS]);

    ValueLabel = CreateWindow("static", "Value: ", WS_VISIBLE | WS_CHILD, 310, 250, 100, 25, hWnd, 0, GetModuleHandle(0), 0);
    Value = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", 0, WS_VISIBLE | WS_CHILD, 400, 247, 100, 20, hWnd, (HMENU)ID_VALUE, GetModuleHandle(0), 0);
    ChangeValue = CreateWindow("button", "Change Value", WS_VISIBLE | WS_CHILD, 510, 245, 100, 25, hWnd, (HMENU)ID_CHANGE_VALUE, GetModuleHandle(0), 0);
    NewScan = CreateWindow("button", "Reset Scan", WS_VISIBLE | WS_CHILD, 510, 275, 100, 25, hWnd, (HMENU)ID_NEW_SCAN, GetModuleHandle(0), 0);

    Pid = CreateWindow("static", "*No Process Selected*", WS_VISIBLE | WS_CHILD, 10, 280, 150, 25, hWnd, (HMENU)ID_PROCESS_ID, GetModuleHandle(0), 0);
    ChoosePid = CreateWindow("button", "Select Process", WS_VISIBLE | WS_CHILD, 175, 275, 100, 25, hWnd, (HMENU)ID_SELECT_PROCESS, GetModuleHandle(0), 0);

    DataSizeLabel = CreateWindow("static", "Type: ", WS_VISIBLE | WS_CHILD, 310, 280, 100, 25, hWnd, 0, GetModuleHandle(0), 0);
    DataSize = CreateWindowEx(WS_EX_CLIENTEDGE, "combobox", 0, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 400, 275, 100, 25, hWnd, (HMENU)ID_VALUE, GetModuleHandle(0), 0);

    for(i = 0; i < ARRAYSIZE(data_types); i++)
    {
        SendMessage(DataSize, CB_ADDSTRING, 0, (LPARAM)data_types[i]);
    }


    Scan = CreateWindow("button", "Scan Memory", WS_VISIBLE | WS_CHILD, 10, 315, 600, 50, hWnd, (HMENU)ID_SCAN, GetModuleHandle(0), 0);

    EnableWindow(Scan, false);

    SendMessage(ChangeValue, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
    SendMessage(ChoosePid, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
    SendMessage(NewScan, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
    SendMessage(Scan, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
    SendMessage(DataSizeLabel, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
    SendMessage(PidLabel, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
    SendMessage(ValueLabel, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
    SendMessage(DataSize, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
    SendMessage(Pid, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
    SendMessage(Value, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
    SendMessage(SearchCondition, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
    SendMessage(SearchConditionLabel, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
}

void CenterWindow(HWND hWnd, HWND Parent)
{
    RECT window;

    GetWindowRect(hWnd, &window);

    DWORD X =  (((GetSystemMetrics(SM_CXSCREEN)) - (window.right)) / 2);
    DWORD Y =  (((GetSystemMetrics(SM_CYSCREEN)) - (window.bottom)) / 2);

    MoveWindow(hWnd, X, Y, window.right - window.left, window.bottom - window.top, true);
}

void ProcessListViewLeftClickEvent(void)
{
    char buffer[256];
    MemoryZero(buffer, sizeof(buffer));

    SelectedItem = (int)SendMessage(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if(SelectedItem != -1 && StringLength(buffer))
    {
        ListView_GetItemText(ListView, SelectedItem, 1, buffer, sizeof(buffer));
        SendMessage(Value, WM_SETTEXT, 0, (LPARAM)buffer);
    }

    (SelectedAddressFrozen()) ? EnableWindow(ChangeValue, false) : EnableWindow(ChangeValue, true);
}

void ProcessListViewRightClickEvent(HWND hWnd)
{
    SelectedItem = (int)SendMessage(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if(SelectedItem != -1)
    {
        POINT pos;

        if(GetCursorPos(&pos))
        {
            char buffer[256];
            MemoryZero(buffer, sizeof(buffer));

            ListView_GetItemText(ListView, SelectedItem, 0, buffer, sizeof(buffer));

            int x = pos.x;
            int y = pos.y;

            unsigned int i;
            bool address_frozen = false;

            for(i = 0; i < addresses_frozen; i++)
            {
                if(StringCompare(frozen_addresses[i], buffer, false))
                {
                    address_frozen = true;
                    break;
                }
            }

            HMENU PopupMenu = CreatePopupMenu();

            if(address_frozen)
            {
                EnableWindow(ChangeValue, false);
                InsertMenu(PopupMenu, 0, MF_STRING, ID_UNFREEZE_VALUE, "Unfreeze Value");
            }

            else
            {
                EnableWindow(ChangeValue, true);
                InsertMenu(PopupMenu, 0, MF_STRING, ID_FREEZE_VALUE, "Freeze Value");
            }

            TrackPopupMenu(PopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, x, y, 0, hWnd, 0);
        }
    }
}

void CleanupAndTerminateApplication(HWND hWnd)
{
    TerminateThread(FreezeThread, 0);
    CloseHandle(FreezeThread);
    TerminateThread(MonitorSelectedProcessThread, 0);
    CloseHandle(MonitorSelectedProcessThread);
    TerminateThread(ScanThread, 0);
    CloseHandle(ScanThread);
    if(scanner) FreeMemoryScanner(scanner);
    DestroyWindow(hWnd);
}

void ProcessFreezeValueButtonEvent(void)
{
    char address[256], value[256];

    MemoryZero(address, sizeof(address));
    MemoryZero(value, sizeof(value));

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address));
    ListView_GetItemText(ListView, SelectedItem, 1, value, sizeof(value));


    unsigned int i;
    bool frozen = false;

    for(i = 0; i < addresses_frozen; i++)
    {
        if(StringCompare(frozen_addresses[i], address, false))
        {
            if(StringCompare(frozen_values[i], value, false))
            {
                frozen = true;
                break;
            }
        }
    }


    if(!frozen)
    {
        if(addresses_frozen < FREEZE_LIMIT)
        {
            MemoryZero(&frozen_addresses[addresses_frozen], sizeof(frozen_addresses[addresses_frozen]));
            CopyMemory(&frozen_addresses[addresses_frozen], address, sizeof(frozen_addresses[addresses_frozen]));

            MemoryZero(&frozen_values[addresses_frozen], sizeof(frozen_values[addresses_frozen]));
            CopyMemory(&frozen_values[addresses_frozen], value, sizeof(frozen_values[addresses_frozen]));

            addresses_frozen++;
        }
    }

    EnableWindow(ChangeValue, false);
}

void ProcessUnfreezeValueButtonEvent(void)
{
    char address[256], value[256];

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address));
    ListView_GetItemText(ListView, SelectedItem, 1, value, sizeof(value));

    unsigned int i;

    for(i = 0; i < addresses_frozen; i++)
    {
        if(StringCompare(frozen_addresses[i], address, false))
        {
            if(StringCompare(frozen_values[i], value, false))
            {
                MemoryZero(&frozen_addresses[i], sizeof(frozen_addresses[i]));
                MemoryZero(&frozen_values[i], sizeof(frozen_values[i]));
            }
        }
    }

    EnableWindow(ChangeValue, true);
}

void CreateAboutDialog(HWND hWnd)
{
    EnableWindow(hWnd, false); 
    MessageBox(hWnd, "A basic memory editing utility.", title, MB_OK);
    EnableWindow(hWnd, true); 
}

void CreateChooseProcessDialogUI(void)
{
    WNDCLASSEX wc;

    wc.cbSize           = sizeof(wc);
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.lpszClassName    = "SelectPidDlg";
    wc.hCursor          = LoadCursor(0, IDC_ARROW);
    wc.lpfnWndProc      = SelectPidDialogProc;
    wc.lpszMenuName     = 0;
    wc.hInstance        = GetModuleHandle(0);
    wc.hbrBackground    = GetSysColorBrush(COLOR_3DFACE);
    wc.hIcon            = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(AppIcon));
    wc.hIconSm          = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(AppIcon));
    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    UnregisterClass(wc.lpszClassName, 0);

    if(RegisterClassEx(&wc))
    {

        PidDlg = CreateWindowEx(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST, wc.lpszClassName, title,
                                WS_SYSMENU | WS_OVERLAPPED, 100,
                                100, 295, 400, 0, 0, 0, 0);


        if(PidDlg)
        {

            ShowWindow(PidDlg, SW_SHOW);
            UpdateWindow(PidDlg);
 
            EnableWindow(MainWindow, false);

            ProcessSelection = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTBOX, 0,
                                              WS_VSCROLL | LBS_NOTIFY | LBS_DISABLENOSCROLL | WS_VISIBLE | WS_CHILD,
                                              10, 10, 270, 300, PidDlg, (HMENU)ID_PROCESSES, GetModuleHandle(0), 0);
  
            ChooseProcess = CreateWindow("button", "Select Process", WS_CHILD | WS_VISIBLE,
                                         10, 310, 270, 50, PidDlg, (HMENU)ID_CHOOSE_PROCESS, GetModuleHandle(0), 0);

            SendMessage(ProcessSelection, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
            SendMessage(ChooseProcess, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));

            EnableWindow(ChooseProcess, false);

            unsigned int i;

            for(i = 0; i < process_count; i++)
            {
                SendMessage(ProcessSelection, LB_ADDSTRING, 0, (LPARAM)processes[i]);
            }
        }
    }
}

void CreateChangeValueDialogUI(void)
{
    if(SelectedItem > -1)
    {
         WNDCLASSEX wc;

         wc.cbSize           = sizeof(wc);
         wc.cbClsExtra       = 0;
         wc.cbWndExtra       = 0;
         wc.lpszClassName    = "ChangeValueDlg";
         wc.hCursor          = LoadCursor(0, IDC_ARROW);
         wc.lpfnWndProc      = ChangeValueDialogProc;
         wc.lpszMenuName     = 0;
         wc.hInstance        = GetModuleHandle(0);
         wc.hbrBackground    = GetSysColorBrush(COLOR_3DFACE);
         wc.hIcon            = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(AppIcon));
         wc.hIconSm          = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(AppIcon));
         wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;


         UnregisterClass(wc.lpszClassName, 0);

         if(RegisterClassEx(&wc))
         {
             ChangeValueDlg = CreateWindowEx(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST, wc.lpszClassName, 
                                             title, WS_SYSMENU | WS_OVERLAPPED, 100,
                                             100, 300, 75, 0, 0, 0, 0);

             if(ChangeValueDlg)
             {
                 ShowWindow(ChangeValueDlg, SW_SHOW);
                 UpdateWindow(ChangeValueDlg);

                 SendMessage(ChangeValueDlgValue, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
                 SendMessage(ChangeValueDlgButton, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));
             }
         }
    }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// LRESULT CALLBACK SelectPidDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

void ProcessListboxChangeEvent(void)
{
    selected_process_index = (int)SendMessage(ProcessSelection, LB_GETCURSEL, 0, 0);

    if(selected_process_index > -1)
    {
        char selected_process[256];

        CopyString(selected_process, pids[selected_process_index], sizeof(selected_process));

        if(StringLength(selected_process))
            EnableWindow(ChooseProcess, true);
        else
            EnableWindow(ChooseProcess, false);
    }
}

void ProcessChooseProcessButtonEvent(void)
{
    if(selected_process_index > -1)
    {
        bool error;
        char selected_process[256], pid[256];

        CopyString(selected_process, pids[selected_process_index], sizeof(selected_process));

        if(StringLength(selected_process))
        {
            error = false;
            SendMessage(Pid, WM_SETTEXT, 0, (LPARAM)pids[selected_process_index]);
        }

        else
        {
            error = true;
            ResetScan(scanner, true, true);
        }

        SendMessage(Pid, WM_GETTEXT, sizeof(pid), (LPARAM)pid);
        unsigned int process_id = (unsigned int)StringToInteger(pid, FMT_INT_DECIMAL);

        HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, process_id);

        if(process)
        {
            if(scanner)
            {
                char data_size[256];
                LRESULT selection_id = SendMessage(DataSize, CB_GETCURSEL, 0, 0);

                if(selection_id > -1) CopyString(data_size, (char *)data_sizes[selection_id], sizeof(data_size));

                if(current_pid != process_id)
                {
                    current_pid = process_id;
                    ResetScan(scanner, false, true);
                    FreeMemoryScanner(scanner);
                    scanner = CreateMemoryScanner(current_pid, (unsigned short)StringToInteger(data_size, FMT_INT_DECIMAL));
                }
            }

            CloseHandle(process);
        }

        else
        {
            error = true;
            EnableWindow(ChoosePid, true);
            if(scanner) ResetScan(scanner, true, true);
        }

        DestroyWindow(PidDlg);
        EnableWindow(MainWindow, true);
        (error) ? EnableWindow(Scan, false) : EnableWindow(Scan, true);
        SetForegroundWindow(MainWindow);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// LRESULT CALLBACK ChangeValueDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

void CreateChangeValueDialogUIChildren(HWND hWnd)
{
    char val[256];
    MemoryZero(val, sizeof(val));
    ListView_GetItemText(ListView, SelectedItem, 1, val, sizeof(val));
    EnableWindow(MainWindow, false);
    ChangeValueDlgValue = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", val, WS_VISIBLE | WS_CHILD, 10, 10, 180, 25, hWnd, (HMENU)ID_CHANGE_DLG_VALUE, 0, 0);
    ChangeValueDlgButton = CreateWindow("button", "Set Value", WS_VISIBLE | WS_CHILD, 200, 10, 85, 25, hWnd, (HMENU)ID_CHANGE_DLG_BUTTON, 0, 0);
}

#endif
