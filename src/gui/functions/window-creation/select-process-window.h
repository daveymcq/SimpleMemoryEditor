#ifndef _SELECT_PROCESS_WINDOW_H
#define _SELECT_PROCESS_WINDOW_H

void CreateChooseProcessWindow(void)
{
    WNDCLASSEX wc;

    wc.cbSize           = sizeof(wc);
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.lpszClassName    = "SelectPidDlg";
    wc.hCursor          = LoadCursor(0, IDC_ARROW);
    wc.lpfnWndProc      = ChooseProcessWindowProc;
    wc.lpszMenuName     = 0;
    wc.hInstance        = GetModuleHandle(0);
    wc.hbrBackground    = GetSysColorBrush(COLOR_3DFACE);
    wc.hIcon            = Icon;
    wc.hIconSm          = Icon;
    wc.style            = CS_HREDRAW | CS_VREDRAW;

    UnregisterClass(wc.lpszClassName, 0);

    if(RegisterClassEx(&wc))
    {
        PidDlg = CreateWindow(wc.lpszClassName, title,
                              WS_POPUP, 100, 100, ChooseProcessWindowWidth,
                              ChooseProcessWindowHeight, 0, 0, 0, 0);


        if(PidDlg)
        {
            ShowWindow(PidDlg, SW_SHOW);
            UpdateWindow(PidDlg);

            EnableWindow(MainWindow, false);

            ChooseProcessWindowCloseButton = CreateWindow("button", 0, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, ChooseProcessWindowWidth - 38, 4,
                                                          34, CustomToolbarHeight - 6, PidDlg, (HMENU)ID_CLOSE_CHOOSE_PROCESS, GetModuleHandle(0), 0);

            ProcessSelection = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTBOX, 0,
                                              WS_VSCROLL | LBS_NOTIFY | LBS_DISABLENOSCROLL | WS_VISIBLE | WS_CHILD,
                                              10, 10 + CustomToolbarHeight, 270, 300, PidDlg, (HMENU)ID_PROCESSES, GetModuleHandle(0), 0);

            ChooseProcess = CreateWindow("button", "Select Process", WS_CHILD | WS_VISIBLE,
                                         10, 310 + CustomToolbarHeight, 270, 50, PidDlg, (HMENU)ID_CHOOSE_PROCESS, GetModuleHandle(0), 0);

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

#endif
