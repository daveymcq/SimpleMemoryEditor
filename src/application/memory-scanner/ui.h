#ifndef _UI_H
#define _UI_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Memory Scanner UI Logic                                     //
///////////////////////////////////////////////////////////////////////////////////////////////////

/* Add scan results to ListView control. */

void AddItemToListView(string address, string value)
{
    static LVITEMA item;

    item.mask        = LVIF_TEXT;
    item.iSubItem    = 0;
    item.pszText     = (string)address;

    SendMessageA(ListView, LVM_INSERTITEM, 0, (LPARAM)&item);

    item.iSubItem    = 1;
    item.pszText     = (string)value;

    SendMessageA(ListView, LVM_SETITEM, 0, (LPARAM)&item);
}

/* Keep ListView control columns fixed. */

DWORD WINAPI DisableListViewResize(void)
{
    while(ListView)
    {
        uint8 column;

        for(column = 0; column < 2; column++)
        {
            ListView_SetColumnWidth(ListView, column, MulDiv(298, ScreenDPI, 96));
        }
    }
}

/* Center a window using the default display. */

void CenterWindow(HWND window)
{
    RECT window_rect;
    DWORD x, y, width, height;

    GetWindowRect(window, &window_rect);

    width = (window_rect.right - window_rect.left);
    height = (window_rect.bottom - window_rect.top);

    x =  ((GetSystemMetrics(SM_CXSCREEN) - width) / 2);
    y =  ((GetSystemMetrics(SM_CYSCREEN) - height) / 2);

    MoveWindow(window, x, y, width, height, true);
}

/* DPI scale the position and size of the controls */

boolean UpdateWindowForDpi(HWND window, uint32 x, uint32 y, uint32 width, uint32 height) 
{ 
    typedef UINT (WINAPI *FP_GETDPIFORWINDOW)(HWND);
    FP_GETDPIFORWINDOW GetDpiForWindow = (FP_GETDPIFORWINDOW)GetProcAddress(GetModuleHandleA("user32"), "GetDpiForWindow");

    if(GetDpiForWindow)
    {
        RECT window_rect;
        GetWindowRect(window, &window_rect);

        ScreenDPI = GetDpiForWindow(window); 

        x = MulDiv(x, ScreenDPI, 96); 
        y = MulDiv(y, ScreenDPI, 96); 
        width = MulDiv(width, ScreenDPI, 96); 
        height = MulDiv(height, ScreenDPI, 96); 

        return SetWindowPos(window, HWND_TOP, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS); 
    }

    return true;
} 

#endif