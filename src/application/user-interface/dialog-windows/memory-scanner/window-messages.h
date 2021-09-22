#ifndef _MEMORY_SCANNER_WINDOW_MESSAGES_H
#define _MEMORY_SCANNER_WINDOW_MESSAGES_H

BOOL GetMemoryScannerWindowMessages(void)
{
    BOOL result;
    MSG message;

    if((result = GetMessage(&message, null, 0, 0)) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return result;
}

#endif
