#ifndef _SELECT_PROCESS_WINDOW_H
#define _SELECT_PROCESS_WINDOW_H

void CreateSelectProcessWindow(void)
{
    WNDCLASSEXA wc;

    wc.cbSize           = sizeof(wc);
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.lpszClassName    = "SelectPidDlg";
    wc.hCursor          = LoadCursorA(Instance, IDC_ARROW);
    wc.lpfnWndProc      = SelectProcessWindowProc;
    wc.lpszMenuName     = null;
    wc.hInstance        = Instance;
    wc.hbrBackground    = GetSysColorBrush(COLOR_3DFACE);
    wc.hIcon            = LoadIconA(Instance, MAKEINTRESOURCEA(AppIcon));
    wc.hIconSm          = LoadIconA(Instance, MAKEINTRESOURCEA(AppIcon));
    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    UnregisterClassA(wc.lpszClassName, Instance);

    if(RegisterClassExA(&wc))
    {
        PidDlg = CreateWindowExA(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST, wc.lpszClassName, 
                                 title, WS_SYSMENU | WS_OVERLAPPED, 100, 
                                 100, 295, 400, null, null, 
                                 Instance, null);

        if(PidDlg)
        {
            uint16 process_counter_index;

            ProcessSelection = CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTBOX, null, WS_VSCROLL | LBS_NOTIFY | 
                                               LBS_DISABLENOSCROLL | WS_VISIBLE | WS_CHILD, 10, 10, 270,
                                               300, PidDlg, (HMENU)ID_PROCESSES, 
                                               Instance, null);

            ChooseProcess = CreateWindowA("button", "Select Process", WS_CHILD | WS_VISIBLE, 10, 310, 
                                          270, 50, PidDlg, (HMENU)ID_CHOOSE_PROCESS, 
                                          Instance, null);

            for(process_counter_index = 0; process_counter_index < (uint16)ProcessCounter; process_counter_index++)
            {
                SendMessageA(ProcessSelection, LB_ADDSTRING, 0, (LPARAM)processes[process_counter_index]);
            }

            EnableWindow(ChooseProcess, false);
            EnableWindow(MainWindow, false);

            SendMessageA(ProcessSelection, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(ChooseProcess, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));

            ShowWindow(PidDlg, SW_SHOW);
        }
    }
}

#endif
