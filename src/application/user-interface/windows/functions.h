#ifndef _WINDOW_FUNCTIONS_H
#define _WINDOW_FUNCTIONS_H

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

#endif
