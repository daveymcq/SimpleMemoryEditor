#ifndef _INITIALIZE_H
#define _INITIALIZE_H

#include "../resources/icon.h"  
#include "lib/NCRT/libc.h"

#include "application/includes.h"

/* Core application code import */

    #include "application/logic/functions.h"

/* End core application code import */

/****************************************************************************************************************** 
  *****************************************************************************************************************/

/* GUI code import */

    #include "application/user-interface/dialog-windows/memory-scanner/window-messages.h"
    
    #include "application/user-interface/dialog-windows/functions.h"

    #include "application/user-interface/dialog-windows/memory-scanner/functions.h"
    #include "application/user-interface/dialog-windows/select-process/functions.h"
    #include "application/user-interface/dialog-windows/change-value/functions.h"

    #include "application/user-interface/dialog-windows/memory-scanner/window.h"
    #include "application/user-interface/dialog-windows/select-process/window.h"
    #include "application/user-interface/dialog-windows/change-value/window.h"

    #include "application/user-interface/dialog-windows/memory-scanner/window-procedure-message-handlers.h"
    #include "application/user-interface/dialog-windows/select-process/window-procedure-message-handlers.h"
    #include "application/user-interface/dialog-windows/change-value/window-procedure-message-handlers.h"

    #include "application/user-interface/dialog-windows/memory-scanner/window-procedure.h"
    #include "application/user-interface/dialog-windows/select-process/window-procedure.h"
    #include "application/user-interface/dialog-windows/change-value/window-procedure.h"

/* End GUI code import */

/****************************************************************************************************************** 
  *****************************************************************************************************************/

/* Initialize global variables. */

BOOL Initialize(HINSTANCE application_instance)
{
    Width = 625;
    Height = 405;
    FirstScanNotRun = true;
    SelectedItem = -1;
    Instance = (application_instance) ? application_instance : GetModuleHandleA(null);

    CommonControls.dwICC = ICC_WIN95_CLASSES;
    CommonControls.dwSize = sizeof(CommonControls);

    return InitCommonControlsEx(&CommonControls);
}

#endif
