#ifndef _ALL_WINDOW_UPDATE_H
#define _ALL_WINDOW_UPDATE_H

void CenterWindow(HWND hWnd)
{
    RECT window;

    GetWindowRect(hWnd, &window);

    DWORD X =  (((GetSystemMetrics(SM_CXSCREEN)) - (window.right)) / 2);
    DWORD Y =  (((GetSystemMetrics(SM_CYSCREEN)) - (window.bottom)) / 2);

    MoveWindow(hWnd, X, Y, window.right - window.left, window.bottom - window.top, true);
}

#endif
