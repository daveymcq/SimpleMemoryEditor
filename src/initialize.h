#ifndef _GLOBAL_DECLARATIONS_H
#define _GLOBAL_DECLARATIONS_H

#include "application/includes.h"

/* Core application code import */

    #include "application/core/functions.h"

/* End core application code import */

/* GUI code import */

    #include "application/user-interface/parent-windows/scanner/window.h"
    #include "application/user-interface/parent-windows/select-process/window.h"
    #include "application/user-interface/parent-windows/change-value/window.h"

    #include "application/user-interface/parent-windows/scanner/window-procedure-event-handlers.h"
    #include "application/user-interface/parent-windows/select-process/window-procedure-event-handlers.h"
    #include "application/user-interface/parent-windows/change-value/window-procedure-event-handlers.h"

    #include "application/user-interface/parent-windows/scanner/window-procedure.h"
    #include "application/user-interface/parent-windows/select-process/window-procedure.h"
    #include "application/user-interface/parent-windows/change-value/window-procedure.h"

/* End GUI code import */

/* Initialize local variables. */

BOOL Initialize(void)
{
    Width = 625;
    Height = 405;
    FirstScanNotRun = true;
    SelectedItem = -1;

    CommonControls.dwICC = ICC_WIN95_CLASSES;
    CommonControls.dwSize = sizeof(CommonControls);

    return InitCommonControlsEx(&CommonControls);
}

#endif
