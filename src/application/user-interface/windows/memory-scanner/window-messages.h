#ifndef _MEMORY_SCANNER_WINDOW_MESSAGES_H
#define _MEMORY_SCANNER_WINDOW_MESSAGES_H

boolean GetMemoryScannerWindowMessages(VOID)
{
    boolean result;
    MSG message;

    if((result = GetMessage(&message, null, 0, 0)) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return result;
}

#endif
