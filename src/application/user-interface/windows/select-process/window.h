#ifndef _SELECT_PROCESS_WINDOW_H
#define _SELECT_PROCESS_WINDOW_H

HWND CreateSelectProcessWindow(void)
{
    WNDCLASSEXA wc;

    wc.cbSize           = sizeof(wc);
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.lpszClassName    = "SelectProcessWindow";
    wc.hCursor          = LoadCursorA(Instance, IDC_ARROW);
    wc.lpfnWndProc      = SelectProcessWindowProc;
    wc.lpszMenuName     = null;
    wc.hInstance        = Instance;
    wc.hbrBackground    = GetSysColorBrush(COLOR_3DFACE);
    wc.hIcon            = LoadIconA(Instance, MAKEINTRESOURCEA(AppIcon));
    wc.hIconSm          = LoadIconA(Instance, MAKEINTRESOURCEA(AppIcon));
    wc.style            = CS_HREDRAW | CS_VREDRAW;

    UnregisterClassA(wc.lpszClassName, Instance);

    if(RegisterClassExA(&wc))
    {
        SelectPidWindow = CreateWindowExA(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST, wc.lpszClassName, 
                                          Title, WS_SYSMENU | WS_OVERLAPPED | WS_VISIBLE, CW_USEDEFAULT, 
                                          CW_USEDEFAULT, 295, 400, null, null, Instance, null);

        if(SelectPidWindow)
        {
            uint16 process_counter_index;

            ProcessSelection = CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTBOX, null, WS_VSCROLL | LBS_NOTIFY | 
                                               LBS_DISABLENOSCROLL | WS_VISIBLE | WS_CHILD, 10, 10, 270,
                                               300, SelectPidWindow, (HMENU)ID_PROCESSES, 
                                               Instance, null);

            ChooseProcess = CreateWindowA("button", "Select Process", WS_CHILD | WS_VISIBLE, 10, 310, 
                                          270, 50, SelectPidWindow, (HMENU)ID_CHOOSE_PROCESS, 
                                          Instance, null);

            for(process_counter_index = 0; process_counter_index < (uint16)ProcessCounter; process_counter_index++)
            {
                SendMessageA(ProcessSelection, LB_ADDSTRING, 0, (LPARAM)Processes[process_counter_index]);
            }

            SendMessageA(ProcessSelection, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            SendMessageA(ChooseProcess, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));

            EnableWindow(ChooseProcess, false);
            EnableWindow(MemoryScannerWindow, false);

            UpdateLayoutForDpi(SelectPidWindow, CW_USEDEFAULT, CW_USEDEFAULT, 295, 400);
            UpdateLayoutForDpi(ProcessSelection, 10, 10, 270, 300);
            UpdateLayoutForDpi(ChooseProcess, 10, 310, 270, 50);
    
            ShowWindow(MemoryScannerWindow, SW_HIDE);
            CenterWindow(SelectPidWindow);

            return SelectPidWindow;
        }
    }

    return null;
}

#endif
