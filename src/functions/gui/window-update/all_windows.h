#ifndef _ALL_WINDOW_UPDATE_H
#define _ALL_WINDOW_UPDATE_H

void CenterWindow(HWND window)
{
    RECT window_rect;
    DWORD x, y, width, height;

    GetWindowRect(window, &window_rect);

    x =  (((GetSystemMetrics(SM_CXSCREEN)) - (window_rect.right)) / 2);
    y =  (((GetSystemMetrics(SM_CYSCREEN)) - (window_rect.bottom)) / 2);
    width = (window_rect.right - window_rect.left);
    height = (window_rect.bottom - window_rect.top);

    MoveWindow(window, x, y, width, height, true);
}

#endif
