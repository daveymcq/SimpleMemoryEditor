#ifndef _MEMORY_SCANNER_WINDOW_MESSAGE_PUMP_H
#define _MEMORY_SCANNER_WINDOW_MESSAGE_PUMP_H

int32 GetMemoryScannerWindowMessages(void)
{
    MSG message;

    while(GetMessage(&message, null, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return (int32)message.wParam;
}

#endif
