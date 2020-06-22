#ifndef _GUI_H
#define _GUI_H

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChooseProcessDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChangeValueDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

void DrawCloseButton(HWND hWnd, HDC hdc)
{
    RECT rc;

    GetClientRect(hWnd,  &rc);

    HBRUSH br = CreateSolidBrush(RGB(200, 30, 30)); 

    FillRect(hdc, &rc, br);
    SetBkColor(hdc, RGB(200, 30, 30)); 
    SetTextColor(hdc, RGB(255, 255, 255));

    TextOut(hdc, 15, 8, "X", 1);
}

void DrawWindowFrame(RECT rect, HDC hdc, HBRUSH brush, DWORD width, DWORD height)
{
    RECT left, right, bottom;

    left.left = 0;
    left.top = rect.bottom;
    left.right = 2;
    left.bottom = height;
    
    FillRect(hdc, &left, brush);

    right.left = width - 2;
    right.top = rect.bottom;
    right.right = width;
    right.bottom = height;
    
    FillRect(hdc, &right, brush);

    bottom.left = 0;
    bottom.top = height - 2;
    bottom.right = width;
    bottom.bottom = height;
    
    FillRect(hdc, &bottom, brush);
}

LRESULT UpdateWindowLocation(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT move = 0;
    DWORD custom_toolbar_height = 34; 

    RECT rc;
    POINT pt;

    GetCursorPos(&pt);
    GetWindowRect(hWnd, &rc);

    rc.bottom = rc.bottom - ((rc.bottom - rc.top) - custom_toolbar_height);

    if(pt.x <= rc.right && pt.x >= rc.left && pt.y <= rc.bottom && pt.y >= rc.top)
    {
        move = DefWindowProc(hWnd, Msg, wParam, lParam);

        if(move == HTCLIENT)
        {
            move = HTCAPTION;
        }
    }

    return move; 
}

void PaintCustomWindowFrame(HWND hWnd, HDC hdc, DWORD X, DWORD Y)
{
    RECT rect;
    HBRUSH brush;
    DWORD custom_toolbar_height = 34; 
    COLORREF background = RGB(57, 60, 64);

    GetClientRect(hWnd, &rect);

    rect.bottom = rect.bottom - ((rect.bottom - rect.top) - custom_toolbar_height);
    brush = CreateSolidBrush(background);

    FillRect(hdc, &rect, brush);

    DrawWindowFrame(rect, hdc, brush, X, Y);

    SetBkColor(hdc, background);
    SetTextColor(hdc, RGB(255, 255, 255));
    TextOut(hdc, 35, 7, title, 22);

    DrawIconEx(hdc, 5, 5, Icon, 20, 20, 0, brush, 0);
}

int CreateMainDialog(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEX wc;

    wc.cbSize = sizeof(wc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(AppIcon));
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(AppIcon));
    wc.hInstance = GetModuleHandle(0);
    wc.lpfnWndProc = MainWndProc;
    wc.lpszClassName = "Main";
    wc.lpszMenuName = 0;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    UnregisterClass(wc.lpszClassName, 0); 

    if(RegisterClassEx(&wc))
    {
        Icon = wc.hIcon;

        MainWindow = CreateWindow(wc.lpszClassName,
                                  title, WS_POPUPWINDOW, 100, 
                                  100, MainWindowWidth,
                                  MainWindowHeight, 0, 0, 
                                  wc.hInstance, 0);

        if(MainWindow)
        {
            ShowWindow(MainWindow, nCmdShow);
            UpdateWindow(MainWindow);

            MSG Msg;

            while(GetMessage(&Msg, 0, 0, 0) > 0)
            {
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
            }

            return (int)Msg.wParam;
        }
    }

    return -1;
}

// LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

void CreateMainDialogUI(HWND hWnd)
{
    static LVCOLUMN Column;
    static NONCLIENTMETRICS metrics;

    static char val_header[] = "Value";
    static char addr_header[] = "Address";

    unsigned short index;
    DWORD custom_toolbar_height = 34;

    MainWindowCloseButton = CreateWindow("button", 0, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, MainWindowWidth - 40, 1,
                               39, custom_toolbar_height - 2, hWnd, (HMENU)ID_CLOSE, GetModuleHandle(0), 0);

    metrics.cbSize = sizeof(metrics);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(metrics), &metrics, 0);
    Font = CreateFontIndirect(&metrics.lfMessageFont);

    ListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, 0, WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SINGLESEL,
                              10, 10 + custom_toolbar_height, 598, 225, hWnd, (HMENU)ID_LISTVIEW, GetModuleHandle(0), 0);

    #ifndef LVS_EX_DOUBLEBUFFER
        #define LVS_EX_DOUBLEBUFFER 0x00010000
    #endif

    SendMessage(ListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

    Column.cx = 298;
    Column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    Column.pszText = addr_header;
    Column.iSubItem = 0;

    SendMessage(ListView, LVM_INSERTCOLUMN, 0, (LPARAM)&Column);

    Column.iSubItem = 1;
    Column.pszText = val_header;

    SendMessage(ListView, LVM_INSERTCOLUMN, 1, (LPARAM)&Column);

    SearchConditionLabel = CreateWindow("static", "Search Condition: ", WS_VISIBLE | WS_CHILD, 10, 245 + custom_toolbar_height, 100, 25, hWnd, 0, GetModuleHandle(0), 0);
    SearchCondition = CreateWindow("combobox", 0, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 150, 245 + custom_toolbar_height, 125, 25, hWnd, (HMENU)ID_SEARCH_CONDITION, GetModuleHandle(0), 0);

    SendMessage(SearchCondition, CB_ADDSTRING, 0, (LPARAM)search_conditions[SEARCH_EQUALS]);

    ValueLabel = CreateWindow("static", "Value: ", WS_VISIBLE | WS_CHILD, 310, 250 + custom_toolbar_height, 100, 25, hWnd, 0, GetModuleHandle(0), 0);
    Value = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", 0, WS_VISIBLE | WS_CHILD, 400, 247 + custom_toolbar_height, 100, 20, hWnd, (HMENU)ID_VALUE, GetModuleHandle(0), 0);
    ChangeValue = CreateWindow("button", "Change Value", WS_VISIBLE | WS_CHILD, 510, 245 + custom_toolbar_height, 100, 25, hWnd, (HMENU)ID_CHANGE_VALUE, GetModuleHandle(0), 0);
    NewScan = CreateWindow("button", "Reset Scan", WS_VISIBLE | WS_CHILD, 510, 275 + custom_toolbar_height, 100, 25, hWnd, (HMENU)ID_NEW_SCAN, GetModuleHandle(0), 0);

    Pid = CreateWindow("static", "*No Process Selected*", WS_VISIBLE | WS_CHILD, 10, 280 + custom_toolbar_height, 150, 25, hWnd, (HMENU)ID_PROCESS_ID, GetModuleHandle(0), 0);
    ChoosePid = CreateWindow("button", "Select Process", WS_VISIBLE | WS_CHILD, 175, 275 + custom_toolbar_height, 100, 25, hWnd, (HMENU)ID_SELECT_PROCESS, GetModuleHandle(0), 0);

    DataSizeLabel = CreateWindow("static", "Type: ", WS_VISIBLE | WS_CHILD, 310, 280 + custom_toolbar_height, 100, 25, hWnd, 0, GetModuleHandle(0), 0);
    DataSize = CreateWindow("combobox", 0, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 400, 275 + custom_toolbar_height, 100, 25, hWnd, (HMENU)ID_VALUE, GetModuleHandle(0), 0);

    for(index = 0; index < ARRAYSIZE(data_types); index++)
    {
        SendMessage(DataSize, CB_ADDSTRING, 0, (LPARAM)data_types[index]);
    }

    Scan = CreateWindow("button", "Scan Memory", WS_VISIBLE | WS_CHILD, 10, 315 + custom_toolbar_height, 600, 50, hWnd, (HMENU)ID_SCAN, GetModuleHandle(0), 0);

    EnableWindow(Scan, false);

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
}

void CenterWindow(HWND hWnd)
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

            ListView_GetItemText(ListView, SelectedItem, 0, buffer, sizeof(buffer));

            int x = pos.x;
            int y = pos.y;

            unsigned int offset;
            bool address_frozen = false;

            for(offset = 0; offset < NumberOfAddressesFrozen; offset++)
            {
                if(StringCompare(frozen_addresses[offset], buffer, false))
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
    WaitForSingleObject(FreezeThread, INFINITE); 
    CloseHandle(FreezeThread);
    TerminateThread(MonitorSelectedProcessThread, 0);
    WaitForSingleObject(MonitorSelectedProcessThread, INFINITE);
    CloseHandle(MonitorSelectedProcessThread);
    TerminateThread(ScanThread, 0);
    WaitForSingleObject(ScanThread, INFINITE);
    CloseHandle(ScanThread);

    if(scanner)
    {
        FreeMemoryScanner(scanner);
    }

    DestroyWindow(hWnd);
}

void ProcessFreezeValueButtonEvent(void)
{
    char address[256] = { 0 };
    char value[512] = { 0 };

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address));
    ListView_GetItemText(ListView, SelectedItem, 1, value, sizeof(value));

    unsigned int offset;
    bool frozen = false;

    for(offset = 0; offset < NumberOfAddressesFrozen; offset++)
    {
        if(StringCompare(frozen_addresses[offset], address, false))
        {
            frozen = true;
            break;
        }
    }

    if(!frozen)
    {
        if(NumberOfAddressesFrozen < FREEZE_LIMIT)
        {
            MemoryZero(&frozen_addresses[NumberOfAddressesFrozen], sizeof(frozen_addresses[NumberOfAddressesFrozen]));
            CopyMemory(&frozen_addresses[NumberOfAddressesFrozen], address, sizeof(frozen_addresses[NumberOfAddressesFrozen]));

            MemoryZero(&frozen_values[NumberOfAddressesFrozen], sizeof(frozen_values[NumberOfAddressesFrozen]));
            CopyMemory(&frozen_values[NumberOfAddressesFrozen], value, sizeof(frozen_values[NumberOfAddressesFrozen]));

            ListView_SetItemText(ListView, SelectedItem, 1, StringConcat(value, " (FROZEN)")); 

            NumberOfAddressesFrozen++;
        }
    }

    EnableWindow(ChangeValue, false);
}

void ProcessUnfreezeValueButtonEvent(void)
{
    char address[256];
    unsigned int offset; 

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address));

    for(offset = 0; offset < NumberOfAddressesFrozen; offset++)
    {
        if(StringCompare(frozen_addresses[offset], address, false))
        {
            ListView_SetItemText(ListView, SelectedItem, 1, frozen_values[offset]);

            MemoryZero(&frozen_addresses[offset], sizeof(frozen_addresses[offset]));
            MemoryZero(&frozen_values[offset], sizeof(frozen_values[offset])); 

            NumberOfAddressesFrozen--; 
        }
    }

    EnableWindow(ChangeValue, true);
}

void CreateChooseProcessDialogUI(void)
{
    DWORD custom_toolbar_height = 34; 
    WNDCLASSEX wc;

    wc.cbSize           = sizeof(wc);
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.lpszClassName    = "SelectPidDlg";
    wc.hCursor          = LoadCursor(0, IDC_ARROW);
    wc.lpfnWndProc      = ChooseProcessDialogProc;
    wc.lpszMenuName     = 0;
    wc.hInstance        = GetModuleHandle(0);
    wc.hbrBackground    = GetSysColorBrush(COLOR_3DFACE);
    wc.hIcon            = Icon; 
    wc.hIconSm          = Icon;
    wc.style            = CS_HREDRAW | CS_VREDRAW;

    UnregisterClass(wc.lpszClassName, 0);

    if(RegisterClassEx(&wc))
    {
        PidDlg = CreateWindowEx(WS_EX_TOPMOST, wc.lpszClassName, title, 
                                WS_POPUP, 100, 100, ChooseProcessWindowWidth, 
                                ChooseProcessWindowHeight, 0, 0, 0, 0);


        if(PidDlg)
        {

            ShowWindow(PidDlg, SW_SHOW);
            UpdateWindow(PidDlg);
 
            EnableWindow(MainWindow, false);

            ChooseProcessWindowCloseButton = CreateWindow("button", 0, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, ChooseProcessWindowWidth - 40, 1,
                                                          39, custom_toolbar_height - 2, PidDlg, (HMENU)ID_CLOSE_CHOOSE_PROCESS, GetModuleHandle(0), 0);

            ProcessSelection = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTBOX, 0,
                                              WS_VSCROLL | LBS_NOTIFY | LBS_DISABLENOSCROLL | WS_VISIBLE | WS_CHILD,
                                              10, 10 + custom_toolbar_height, 270, 300, PidDlg, (HMENU)ID_PROCESSES, GetModuleHandle(0), 0);
  
            ChooseProcess = CreateWindow("button", "Select Process", WS_CHILD | WS_VISIBLE,
                                         10, 310 + custom_toolbar_height, 270, 50, PidDlg, (HMENU)ID_CHOOSE_PROCESS, GetModuleHandle(0), 0);

            SendMessage(ProcessSelection, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessage(ChooseProcess, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));

            EnableWindow(ChooseProcess, false);

            unsigned int index;

            for(index = 0; index < ProcessCounter; index++)
            {
                SendMessage(ProcessSelection, LB_ADDSTRING, 0, (LPARAM)processes[index]);
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// LRESULT CALLBACK ChangeValueDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

void CreateChangeValueDialogUI(void)
{
    if(SelectedItem > -1)
    {
         DWORD custom_toolbar_height = 34; 
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
         wc.hIcon            = Icon;
         wc.hIconSm          = Icon;
         wc.style            = CS_HREDRAW | CS_VREDRAW;


         UnregisterClass(wc.lpszClassName, 0);

         if(RegisterClassEx(&wc))
         {
             ChangeValueDlg = CreateWindowEx(WS_EX_TOPMOST, wc.lpszClassName, title, WS_POPUP, 
                                             100, 100, ChangeValueWindowWidth, 
                                             ChangeValueWindowHeight, 0, 0, 0, 0);

             if(ChangeValueDlg)
             {
                 char val[256]; 

                 EnableWindow(MainWindow, false); 
                 ShowWindow(ChangeValueDlg, SW_SHOW);
                 UpdateWindow(ChangeValueDlg);

                 ChangeValueWindowCloseButton = CreateWindow("button", 0, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, ChangeValueWindowWidth - 40, 1,
                                                             39, custom_toolbar_height - 2, ChangeValueDlg, (HMENU)ID_CLOSE_CHANGE_VALUE, GetModuleHandle(0), 0);

                 SendMessage(ChangeValueDlgValue, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
                 SendMessage(ChangeValueDlgButton, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));

                 ListView_GetItemText(ListView, SelectedItem, 1, val, sizeof(val)); 

                 ChangeValueDlgValue = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", val, WS_VISIBLE | WS_CHILD, 10, 10 + custom_toolbar_height, 180, 25, ChangeValueDlg, (HMENU)ID_CHANGE_DLG_VALUE, 0, 0);
                 ChangeValueDlgButton = CreateWindow("button", "Set Value", WS_VISIBLE | WS_CHILD, 200, 10 + custom_toolbar_height, 85, 25, ChangeValueDlg, (HMENU)ID_CHANGE_DLG_BUTTON, 0, 0);

                 SendMessage(ChangeValueDlgValue, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
                 SendMessage(ChangeValueDlgButton, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0)); 
             }
         }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// LRESULT CALLBACK SelectPidDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

void ProcessListboxChangeEvent(void)
{
    IndexOfSelectedProcess = (int)SendMessage(ProcessSelection, LB_GETCURSEL, 0, 0);

    if(IndexOfSelectedProcess > -1)
    {
        char selected_process[256];

        CopyString(selected_process, pids[IndexOfSelectedProcess], sizeof(selected_process));

        if(StringLength(selected_process))
            EnableWindow(ChooseProcess, true);
        else
            EnableWindow(ChooseProcess, false);
    }
}

void ProcessChooseProcessButtonEvent(void)
{
    if(IndexOfSelectedProcess > -1)
    {
        bool error;
        char selected_process[256];
        char pid[256];

        CopyString(selected_process, pids[IndexOfSelectedProcess], sizeof(selected_process));

        if(StringLength(selected_process))
        {
            CopyString(pid, pids[IndexOfSelectedProcess], sizeof(pid));
            CopyString(PID, pid, sizeof(PID));
            SendMessage(Pid, WM_SETTEXT, 0, (LPARAM)processes[IndexOfSelectedProcess]);

            error = false; 
        }

        else
        {
            ResetScan(scanner, true, true);
            error = true; 
        }

        unsigned int process_id = (unsigned int)StringToInteger(pid, FMT_INT_DECIMAL);

        HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, process_id);

        if(process)
        {
            if(scanner)
            {
                char data_size[256];

                LRESULT selection_id = SendMessage(DataSize, CB_GETCURSEL, 0, 0);

                if(selection_id > -1) CopyString(data_size, (char *)data_sizes[selection_id], sizeof(data_size));

                if(CurrentProcess != process_id)
                {
                    CurrentProcess = process_id;
                    ResetScan(scanner, false, true);
                    FreeMemoryScanner(scanner);
                    scanner = CreateMemoryScanner(CurrentProcess, (unsigned short)StringToInteger(data_size, FMT_INT_DECIMAL));
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


// Populates the ListView with the results of the scan.

void AddItemToListView(void *address, const char *value)
{
    static LVITEM Item;

    Item.mask        = LVIF_TEXT;
    Item.iSubItem    = 0;
    Item.pszText     = (char *)address;

    SendMessage(ListView, LVM_INSERTITEM, 0, (LPARAM)&Item);

    Item.iSubItem    = 1;
    Item.pszText     = (char *)value;

    SendMessage(ListView, LVM_SETITEM, 0, (LPARAM)&Item);
}

#include "window-procedures/main-window-procedures.h"
#include "window-procedures/choose-process-window-procedures.h"
#include "window-procedures/change-value-window-procedures.h"

#endif
