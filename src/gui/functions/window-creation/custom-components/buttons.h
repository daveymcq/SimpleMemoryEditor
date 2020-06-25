#ifndef _CUSTOM_BUTTONS
#define _CUSTOM_BUTTONS

void DrawCustomButton(HWND hWnd, HDC hdc, DWORD ID, DWORD x, DWORD y, DWORD width, DWORD height)
{

    RECT window;
    HPEN hpen;
    char text[256];
    unsigned int offset;

    SendMessage(hWnd, WM_SETFONT, (WPARAM)Font, MAKELPARAM(true, 0)); 
    GetClientRect(hWnd,  &window); 
    SendDlgItemMessage(hWnd, ID, WM_GETTEXT, (WPARAM)sizeof(text), (LPARAM)text); 

    for(offset = 0; offset < height; offset++)
    {
        hpen = CreatePen(PS_SOLID, 4, RGB(120 - offset, 0, 0));
        SelectObject(hdc, hpen);
        Rectangle(hdc, 0, 0 + offset, width + 2, 1 + offset);
        DeleteObject(hpen);
    }

    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkColor(hdc, RGB(130, 0, 0)); 
    SetBkMode(hdc, TRANSPARENT);

    TextOut(hdc, x, y, text, StringLength(text)); 
}

#endif

