#ifndef _CUSTOM_WINDOW_FRAME
#define _CUSTOM_WINDOW_FRAME

void DrawCloseButton(HWND hWnd, HDC hdc)
{
    RECT window;

    GetClientRect(hWnd,  &window);

    HBRUSH br = CreateSolidBrush(RGB(200, 30, 30));

    FillRect(hdc, &window, br);
    SetBkColor(hdc, RGB(200, 30, 30));
    SetTextColor(hdc, RGB(255, 255, 255));

    TextOut(hdc, 13, 4, "X", 1);
}

void DrawWindowFrame(RECT rect, HDC hdc, HBRUSH brush, DWORD width, DWORD height)
{
    RECT left, right, bottom;

    left.left = 0;
    left.top = rect.bottom;
    left.right = 4;
    left.bottom = height;

    FillRect(hdc, &left, brush);

    right.left = width - 4;
    right.top = rect.bottom;
    right.right = width;
    right.bottom = height;

    FillRect(hdc, &right, brush);

    bottom.left = 0;
    bottom.top = height - 4;
    bottom.right = width;
    bottom.bottom = height;

    FillRect(hdc, &bottom, brush);
}

LRESULT UpdateWindowLocation(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT move = 0;

    RECT rc;
    POINT pt;

    GetCursorPos(&pt);
    GetWindowRect(hWnd, &rc);

    rc.bottom = rc.bottom - ((rc.bottom - rc.top) - CustomToolbarHeight);

    if(pt.x <= rc.right && pt.x >= rc.left && pt.y <= rc.bottom && pt.y >= rc.top)
    {
        move = DefWindowProc(hWnd, Msg, wParam, lParam);

        if(move == HTCLIENT)
        {
            move = HTCAPTION;
        }
    }

    return move;
}

void PaintCustomWindowFrame(HWND hWnd, HDC hdc, DWORD X, DWORD Y)
{
    RECT window;
    HBRUSH brush;

    COLORREF background = RGB(51, 51, 51);

    GetClientRect(hWnd, &window);

    window.bottom = window.bottom - ((window.bottom - window.top - 2) - CustomToolbarHeight);

    brush = CreateSolidBrush(background);

    FillRect(hdc, &window, brush);

    DrawWindowFrame(window, hdc, brush, X, Y);

    SetBkColor(hdc, background);
    SetTextColor(hdc, RGB(255, 255, 255));
    TextOut(hdc, 35, 7, title, 22);

    DrawIconEx(hdc, 5, 5, Icon, 20, 20, 0, brush, 0);
}

void CenterWindow(HWND hWnd)
{
    RECT window;

    GetWindowRect(hWnd, &window);

    DWORD X =  (((GetSystemMetrics(SM_CXSCREEN)) - (window.right)) / 2);
    DWORD Y =  (((GetSystemMetrics(SM_CYSCREEN)) - (window.bottom)) / 2);

    MoveWindow(hWnd, X, Y, window.right - window.left, window.bottom - window.top, true);
}

#endif
