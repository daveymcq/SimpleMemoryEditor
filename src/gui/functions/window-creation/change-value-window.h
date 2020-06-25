#ifndef _CHANGE_VALUE_WINDOW_H
#define _CHANGE_VALUE_WINDOW_H

void CreateChangeValueDialog(void)
{
    if(SelectedItem > -1)
    {
        WNDCLASSEX wc;

        wc.cbSize           = sizeof(wc);
        wc.cbClsExtra       = 0;
        wc.cbWndExtra       = 0;
        wc.lpszClassName    = "ChangeValueDlg";
        wc.hCursor          = LoadCursor(0, IDC_ARROW);
        wc.lpfnWndProc      = ChangeValueWindowProc;
        wc.lpszMenuName     = 0;
        wc.hInstance        = GetModuleHandle(0);
        wc.hbrBackground    = GetSysColorBrush(COLOR_3DFACE);
        wc.hIcon            = Icon;
        wc.hIconSm          = Icon;
        wc.style            = CS_HREDRAW | CS_VREDRAW;


        UnregisterClass(wc.lpszClassName, 0);

        if(RegisterClassEx(&wc))
        {
            ChangeValueDlg = CreateWindow(wc.lpszClassName, title, WS_POPUP,
                                          100, 100, ChangeValueWindowWidth,
                                          ChangeValueWindowHeight, 0, 0, 0, 0);

            if(ChangeValueDlg)
            {
                char val[256];

                EnableWindow(MainWindow, false);
                ShowWindow(ChangeValueDlg, SW_SHOW);
                UpdateWindow(ChangeValueDlg);

                ChangeValueWindowCloseButton = CreateWindow("button", 0, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, ChangeValueWindowWidth - 38, 4,
                                                            34, CustomToolbarHeight - 6, ChangeValueDlg, (HMENU)ID_CLOSE_CHANGE_VALUE, GetModuleHandle(0), 0);


                SendMessage(ChangeValueDlgValue, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
                SendMessage(ChangeValueDlgButton, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));

                ListView_GetItemText(ListView, SelectedItem, 1, val, sizeof(val));

                ChangeValueDlgValue = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", val, WS_VISIBLE | WS_CHILD, 10, 10 + CustomToolbarHeight, 180, 25, ChangeValueDlg, (HMENU)ID_CHANGE_DLG_VALUE, 0, 0);
                ChangeValueDlgButton = CreateWindow("button", "Set Value", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 200, 10 + CustomToolbarHeight, 85, 25, ChangeValueDlg, (HMENU)ID_CHANGE_DLG_BUTTON, 0, 0);

                SendMessage(ChangeValueDlgValue, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
                SendMessage(ChangeValueDlgButton, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
            }
        }
    }
}

#endif
