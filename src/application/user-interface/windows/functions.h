#ifndef _WINDOW_FUNCTIONS_H
#define _WINDOW_FUNCTIONS_H

/* Center a window using the default display. */

VOID CenterWindow(HWND window)
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

VOID UpdateLayoutForDpi(HWND window, DWORD x, DWORD y, DWORD width, DWORD height) 
{ 
    typedef UINT (WINAPI *FP_GETDPIFORWINDOW) (HWND);
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

        SetWindowPos(window, window, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE); 
    }
} 

#endif
