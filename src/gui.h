#ifndef _GUI_H
#define _GUI_H

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SelectPidDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChangeValueDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

char val_header[] = "Value";
char addr_header[] = "Address";
static unsigned short i;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

void CreateMainDialogUI(HWND hWnd)
{
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


    memset(&Column, 0, sizeof(Column));

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

    EnableWindow(Scan, 0);

    SendMessage(ChangeValue, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
    SendMessage(ChoosePid, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
    SendMessage(NewScan, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
    SendMessage(Scan, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
    SendMessage(DataSizeLabel, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
    SendMessage(PidLabel, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
    SendMessage(ValueLabel, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
    SendMessage(DataSize, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
    SendMessage(Pid, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
    SendMessage(Value, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
    SendMessage(SearchCondition, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
    SendMessage(SearchConditionLabel, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
}


void ProcessListViewLeftClickEvent(void)
{
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    SelectedItem = (int)SendMessage(ListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

    if(SelectedItem != -1 && lstrlen(buffer))
    {
        ListView_GetItemText(ListView, SelectedItem, 1, buffer, sizeof(buffer));
        SendMessage(Value, WM_SETTEXT, 0, (LPARAM)buffer);
    }

    (SelectedAddressFrozen()) ? EnableWindow(ChangeValue, FALSE) : EnableWindow(ChangeValue, TRUE);
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
            memset(buffer, 0, sizeof(buffer));

            ListView_GetItemText(ListView, SelectedItem, 0, buffer, sizeof(buffer));

            int x = pos.x;
            int y = pos.y;

            unsigned int i;
            BOOL address_frozen = FALSE;

            for(i = 0; i < addresses_frozen; i++)
            {
                if(lstrcmp(frozen_addresses[i], buffer) == 0)
                {
                    address_frozen = TRUE;
                    break;
                }
            }

            HMENU PopupMenu = CreatePopupMenu();

            if(address_frozen)
            {
                EnableWindow(ChangeValue, FALSE);
                InsertMenu(PopupMenu, 0, MF_STRING, ID_UNFREEZE_VALUE, "Unfreeze Value");
            }
            else
            {
                EnableWindow(ChangeValue, TRUE);
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

    memset(address, 0, sizeof(address));
    memset(value, 0, sizeof(value));

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address));
    ListView_GetItemText(ListView, SelectedItem, 1, value, sizeof(value));


    unsigned int i;
    BOOL frozen = FALSE;

    for(i = 0; i < addresses_frozen; i++)
    {
        if(strcmp(frozen_addresses[i], address) == 0)
        {
            if(strcmp(frozen_values[i], value) == 0)
            {
                frozen = TRUE;
                break;
            }
        }
    }


    if(!frozen)
    {
        if(addresses_frozen < FREEZE_LIMIT)
        {
            memset(&frozen_addresses[addresses_frozen], 0, sizeof(frozen_addresses[addresses_frozen]));
            memcpy(&frozen_addresses[addresses_frozen], address, sizeof(frozen_addresses[addresses_frozen]));

            memset(&frozen_values[addresses_frozen], 0, sizeof(frozen_values[addresses_frozen]));
            memcpy(&frozen_values[addresses_frozen], value, sizeof(frozen_values[addresses_frozen]));

            addresses_frozen++;
        }
    }

    EnableWindow(ChangeValue, FALSE);
}


void ProcessUnfreezeValueButtonEvent(void)
{
    char address[256], value[256];

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address));
    ListView_GetItemText(ListView, SelectedItem, 1, value, sizeof(value));

    unsigned int i;

    for(i = 0; i < addresses_frozen; i++)
    {
        if(strcmp(frozen_addresses[i], address) == 0)
        {
            if(strcmp(frozen_values[i], value) == 0)
            {
                memset(&frozen_addresses[i], 0, sizeof(frozen_addresses[i]));
                memset(&frozen_values[i], 0, sizeof(frozen_values[i]));
            }
        }
    }

    EnableWindow(ChangeValue, TRUE);
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

    RegisterClassEx(&wc);

    PidDlg = CreateWindowEx(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST, wc.lpszClassName, title,
                            WS_VISIBLE | WS_SYSMENU | WS_OVERLAPPED, CW_USEDEFAULT,
                            CW_USEDEFAULT, 295, 400, 0, 0, 0, 0);


    if(PidDlg)
    {
       EnableWindow(MainWindow, FALSE);
       ProcessSelection = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTBOX, 0,
                                       WS_VSCROLL | LBS_NOTIFY | LBS_DISABLENOSCROLL | WS_VISIBLE | WS_CHILD,
                                       10, 10, 270, 300, PidDlg, (HMENU)ID_PROCESSES, GetModuleHandle(0), 0);

       ChooseProcess = CreateWindow("button", "Select Process", WS_CHILD | WS_VISIBLE,
                                    10, 310, 270, 50, PidDlg, (HMENU)ID_CHOOSE_PROCESS, GetModuleHandle(0), 0);
       SendMessage(ProcessSelection, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
       SendMessage(ChooseProcess, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));

       EnableWindow(ChooseProcess, 0);

       for(i = 0; i < ARRAYSIZE(processes); i++)
       {
           SendMessage(ProcessSelection, LB_ADDSTRING, 0, (LPARAM)processes[i]);
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


         RegisterClassEx(&wc);

         ChangeValueDlg = CreateWindowEx(WS_EX_DLGMODALFRAME, wc.lpszClassName, title,
                                         WS_VISIBLE | WS_SYSMENU | WS_OVERLAPPED, CW_USEDEFAULT,
                                         CW_USEDEFAULT, 300, 75, 0, 0, 0, 0);

         if(ChangeValueDlg)
         {
              SendMessage(ChangeValueDlgValue, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
              SendMessage(ChangeValueDlgButton, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
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
        memset(selected_process, 0, sizeof(selected_process)); 

        _snprintf(selected_process, sizeof(selected_process), "%s", pids[selected_process_index]);
        
        if(lstrlen(selected_process)) 
            EnableWindow(ChooseProcess, TRUE);
        else
            EnableWindow(ChooseProcess, FALSE);
    }
}

void ProcessChooseProcessButtonEvent(void)
{
    if(selected_process_index > -1)
    {
        BOOL error;
        char selected_process[255], pid[255];

        _snprintf(selected_process, sizeof(selected_process), "%s", pids[selected_process_index]);

        if(lstrlen(selected_process))
        {
            error = FALSE;
            SendMessage(Pid, WM_SETTEXT, 0, (LPARAM)pids[selected_process_index]);
        }
        else
        {
            error = TRUE;
            ResetScan(scanner, TRUE, TRUE);
        }

        SendMessage(Pid, WM_GETTEXT, sizeof(pid), (LPARAM)pid);
        unsigned int process_id = (unsigned int)atoi(pid);

        HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);

        if(process)
        {
            if(scanner)
            {
                char data_size[255];
                LRESULT selection_id = SendMessage(DataSize, CB_GETCURSEL, 0, 0);

                if(selection_id > -1) _snprintf(data_size, sizeof(data_size), "%s", data_sizes[selection_id]);

                if(current_pid != process_id)
                {
                    current_pid = process_id;
                    ResetScan(scanner, FALSE, TRUE);
                    FreeMemoryScanner(scanner);
                    scanner = CreateMemoryScanner(current_pid, (unsigned short)atoi(data_size));
                }
            }

            CloseHandle(process);
        }
        else
        {
            error = TRUE;
            EnableWindow(ChoosePid, TRUE);
            if(scanner) ResetScan(scanner, TRUE, TRUE);
        }

        DestroyWindow(PidDlg);
        EnableWindow(MainWindow, TRUE);
        (error) ? EnableWindow(Scan, FALSE) : EnableWindow(Scan, TRUE);
        SetForegroundWindow(MainWindow);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// LRESULT CALLBACK ChangeValueDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


void CreateChangeValueDialogUIChildren(HWND hWnd)
{
    char val[256];
    memset(val, 0, sizeof(val));
    ListView_GetItemText(ListView, SelectedItem, 1, val, sizeof(val));
    EnableWindow(MainWindow, FALSE);
    ChangeValueDlgValue = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", val, WS_VISIBLE | WS_CHILD, 10, 10, 180, 25, hWnd, (HMENU)ID_CHANGE_DLG_VALUE, 0, 0);
    ChangeValueDlgButton = CreateWindow("button", "Set Value", WS_VISIBLE | WS_CHILD, 200, 10, 85, 25, hWnd, (HMENU)ID_CHANGE_DLG_BUTTON, 0, 0);
}

#endif
