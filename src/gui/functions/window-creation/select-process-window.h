#ifndef _SELECT_PROCESS_WINDOW_H
#define _SELECT_PROCESS_WINDOW_H

void CreateSelectProcessWindow(void)
{
    WNDCLASSEX wc;
    HINSTANCE hInstance;

    hInstance = GetModuleHandle(0);

    wc.cbSize           = sizeof(wc);
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.lpszClassName    = "SelectPidDlg";
    wc.hCursor          = LoadCursor(0, IDC_ARROW);
    wc.lpfnWndProc      = SelectProcessWindowProc;
    wc.lpszMenuName     = 0;
    wc.hInstance        = hInstance;
    wc.hbrBackground    = GetSysColorBrush(COLOR_3DFACE);
    wc.hIcon            = LoadIcon(hInstance, MAKEINTRESOURCE(AppIcon));
    wc.hIconSm          = LoadIcon(hInstance, MAKEINTRESOURCE(AppIcon));
    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    UnregisterClass(wc.lpszClassName, 0);

    if(RegisterClassEx(&wc))
    {
        PidDlg = CreateWindowEx(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST, wc.lpszClassName, 
                                title, WS_SYSMENU | WS_OVERLAPPED, 100, 
                                100, 295, 400, 0, 0, hInstance, 0);

        if(PidDlg)
        {
            uint32 index;

            ProcessSelection = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTBOX, 0, WS_VSCROLL | LBS_NOTIFY | 
                                              LBS_DISABLENOSCROLL | WS_VISIBLE | WS_CHILD, 10, 10, 270,
                                              300, PidDlg, (HMENU)ID_PROCESSES, hInstance, 0);

            ChooseProcess = CreateWindow("button", "Select Process", WS_CHILD | WS_VISIBLE, 10, 310, 
                                         270, 50, PidDlg, (HMENU)ID_CHOOSE_PROCESS, hInstance, 0);

            SendMessage(ProcessSelection, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessage(ChooseProcess, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));

            EnableWindow(ChooseProcess, false);

            for(index = 0; index < ProcessCounter; index++)
            {
                SendMessage(ProcessSelection, LB_ADDSTRING, 0, (LPARAM)processes[index]);
            }

            EnableWindow(MainWindow, false); 

            ShowWindow(PidDlg, SW_SHOW);
            UpdateWindow(PidDlg); 
        }
    }
}

#endif
