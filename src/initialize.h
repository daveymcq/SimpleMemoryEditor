#ifndef _GLOBAL_DECLARATIONS_H
#define _GLOBAL_DECLARATIONS_H

#include "application/includes.h"

/* Core application code import */

    #include "application/core/functions.h"

/* End core application code import */

/* GUI code import */

    #include "application/user-interface/parent-windows/memory-scanner/window-message-pump.h"
    
    #include "application/user-interface/parent-windows/memory-scanner/window.h"
    #include "application/user-interface/parent-windows/select-process/window.h"
    #include "application/user-interface/parent-windows/change-value/window.h"

    #include "application/user-interface/parent-windows/memory-scanner/window-procedure-message-handlers.h"
    #include "application/user-interface/parent-windows/select-process/window-procedure-message-handlers.h"
    #include "application/user-interface/parent-windows/change-value/window-procedure-message-handlers.h"

    #include "application/user-interface/parent-windows/memory-scanner/window-procedure.h"
    #include "application/user-interface/parent-windows/select-process/window-procedure.h"
    #include "application/user-interface/parent-windows/change-value/window-procedure.h"

/* End GUI code import */

/* Initialize global variables. */

BOOL Initialize(HINSTANCE instance)
{
    Width = 625;
    Height = 405;
    FirstScanNotRun = true;
    SelectedItem = -1;
    Instance = (instance) ? instance : GetModuleHandleA(null);

    CommonControls.dwICC = ICC_WIN95_CLASSES;
    CommonControls.dwSize = sizeof(CommonControls);

    return InitCommonControlsEx(&CommonControls);
}

#endif
