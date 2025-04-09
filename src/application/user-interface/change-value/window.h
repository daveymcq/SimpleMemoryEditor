#ifndef _CHANGE_VALUE_WINDOW_H
#define _CHANGE_VALUE_WINDOW_H

HWND CreateChangeValueWindow(void)
{
    if(SelectedItem > -1)
    {
        WNDCLASSEXA wc;

        wc.cbSize           = sizeof(wc);
        wc.cbClsExtra       = 0;
        wc.cbWndExtra       = 0;
        wc.lpszClassName    = "ChangeValueWindow";
        wc.hCursor          = LoadCursorA(Instance, IDC_ARROW);
        wc.lpfnWndProc      = ChangeValueWindowProc;
        wc.lpszMenuName     = null;
        wc.hInstance        = Instance;
        wc.hbrBackground    = GetSysColorBrush(COLOR_3DFACE);
        wc.hIcon            = LoadIconA(Instance, MAKEINTRESOURCEA(AppIcon));
        wc.hIconSm          = LoadIconA(Instance, MAKEINTRESOURCEA(AppIcon));
        wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

        UnregisterClassA(wc.lpszClassName, Instance);

        if(RegisterClassExA(&wc))
        {
            ChangeValueWindow = CreateWindowExA(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST, wc.lpszClassName, 
                                                Title, WS_SYSMENU | WS_OVERLAPPED | WS_VISIBLE, 
                                                0, 0, 0, 0, null, null, 
                                                Instance, null);

            if(ChangeValueWindow)
            {
                int8 value[256];

                ListView_GetItemText(ListView, SelectedItem, 1, value, sizeof(value) - 1);

                ChangeValueWindowNewValue = CreateWindowA("edit", value, WS_VISIBLE | WS_CHILD | WS_BORDER, 
                                                           10, 10, 180, 25, ChangeValueWindow, 
                                                           (HMENU)ID_CHANGE_VALUE_WINDOW, 
                                                           null, null);

                ChangeValueWindowButton = CreateWindowA("button", "Save", WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, ChangeValueWindow, 
                                                        (HMENU)ID_CHANGE_VALUE_WINDOW_BUTTON, 
                                                        null, null);

                SendMessageA(ChangeValueWindowNewValue, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));
                SendMessageA(ChangeValueWindowButton, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0));

                UpdateWindowForDpi(ChangeValueWindow, CW_USEDEFAULT, CW_USEDEFAULT, 300, 75);
                UpdateWindowForDpi(ChangeValueWindowNewValue, 10, 10, 180, 25);
                UpdateWindowForDpi(ChangeValueWindowButton, 200, 10, 85, 25);

                EnableWindow(MemoryScannerWindow, false);
                ShowWindow(MemoryScannerWindow, SW_HIDE);
                CenterWindow(ChangeValueWindow);

                return ChangeValueWindow;
            }
        }
    }

    return null;
}

#endif
