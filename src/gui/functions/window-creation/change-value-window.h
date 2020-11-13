#ifndef _CHANGE_VALUE_WINDOW_H
#define _CHANGE_VALUE_WINDOW_H

void CreateChangeValueWindow(void)
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
                int8 val[256];

                ListView_GetItemText(ListView, SelectedItem, 1, val, sizeof(val));

                ChangeValueDlgValue = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", val, WS_VISIBLE | WS_CHILD, 10, 10, 180, 25, ChangeValueDlg, (HMENU)ID_CHANGE_DLG_VALUE, 0, 0);
                ChangeValueDlgButton = CreateWindow("button", "Set Value", WS_VISIBLE | WS_CHILD, 200, 10, 85, 25, ChangeValueDlg, (HMENU)ID_CHANGE_DLG_BUTTON, 0, 0); 

                SendMessage(ChangeValueDlgValue, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
                SendMessage(ChangeValueDlgButton, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
                SendMessage(ChangeValueDlgValue, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
                SendMessage(ChangeValueDlgButton, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));

                EnableWindow(MainWindow, false);
                ShowWindow(ChangeValueDlg, SW_SHOW);
                UpdateWindow(ChangeValueDlg); 
            }
        }
    }
}


#endif
