#ifndef _ALL_WINDOW_UPDATE_H
#define _ALL_WINDOW_UPDATE_H

void CenterWindow(HWND hWnd)
{
    RECT window;
    DWORD X, Y;

    GetWindowRect(hWnd, &window);

    X =  (((GetSystemMetrics(SM_CXSCREEN)) - (window.right)) / 2);
    Y =  (((GetSystemMetrics(SM_CYSCREEN)) - (window.bottom)) / 2);

    MoveWindow(hWnd, X, Y, window.right - window.left, window.bottom - window.top, true);
}

#endif
