#ifndef _CHANGE_VALUE_WINDOW_H
#define _CHANGE_VALUE_WINDOW_H

void CreateChangeValueWindow(void)
{
    if(SelectedItem > -1)
    {
        WNDCLASSEXA wc;

        wc.cbSize           = sizeof(wc);
        wc.cbClsExtra       = 0;
        wc.cbWndExtra       = 0;
        wc.lpszClassName    = "ChangeValueDlg";
        wc.hCursor          = LoadCursorA(null, IDC_ARROW);
        wc.lpfnWndProc      = ChangeValueWindowProc;
        wc.lpszMenuName     = null;
        wc.hInstance        = Instance;
        wc.hbrBackground    = GetSysColorBrush(COLOR_3DFACE);
        wc.hIcon            = LoadIconA(Instance, MAKEINTRESOURCE(AppIcon));
        wc.hIconSm          = LoadIconA(Instance, MAKEINTRESOURCE(AppIcon));
        wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;


        UnregisterClassA(wc.lpszClassName, Instance);

        if(RegisterClassExA(&wc))
        {
            ChangeValueDlg = CreateWindowExA(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST, wc.lpszClassName, 
                                             title, WS_SYSMENU | WS_OVERLAPPED, 100, 
                                             100, 300, 75, null, null, 
                                             Instance, null);

            if(ChangeValueDlg)
            {
                int8 value[MAX_PATH];

                ListView_GetItemText(ListView, SelectedItem, 1, value, sizeof(value) - 1);

                ChangeValueDlgValue = CreateWindowExA(WS_EX_CLIENTEDGE, "edit", value, WS_VISIBLE | WS_CHILD, 
                                                      10, 10, 180, 25, ChangeValueDlg, 
                                                      (HMENU)ID_CHANGE_DLG_VALUE, 
                                                      null, null);

                ChangeValueDlgButton = CreateWindowA("button", "Set Value", WS_VISIBLE | WS_CHILD, 
                                                     200, 10, 85, 25, ChangeValueDlg, 
                                                     (HMENU)ID_CHANGE_DLG_BUTTON, 
                                                     null, null);

                EnableWindow(MainWindow, false);

                SendMessageA(ChangeValueDlgValue, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
                SendMessageA(ChangeValueDlgButton, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
                SendMessageA(ChangeValueDlgValue, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
                SendMessageA(ChangeValueDlgButton, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
                
                ShowWindow(ChangeValueDlg, SW_SHOW); 
            }
        }
    }
}


#endif
