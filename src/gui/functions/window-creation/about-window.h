#ifndef _ABOUT_WINDOW_H
#define _ABOUT_WINDOW_H

void CreateAboutDialog(HWND hWnd)
{
    EnableWindow(hWnd, false); 
    MessageBox(hWnd, "A basic memory editing utility.", title, MB_OK);
    EnableWindow(hWnd, true); 
}

#endif
