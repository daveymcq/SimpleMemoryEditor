#ifndef _INITIALIZE_H
#define _INITIALIZE_H

#include "resources/icon.h"  
#include "libraries/NCRT/ncrt.h"

#include "application/includes.h"

/*  application logic code import */

    #include "application/functions.h"

/* End application logic import */

/****************************************************************************************************************** 
 ******************************************************************************************************************/

/* GUI code import */

    #include "application/user-interface/windows/functions.h"
    #include "application/user-interface/windows/memory-scanner/window-messages.h"

    #include "application/user-interface/windows/memory-scanner/functions.h"
    #include "application/user-interface/windows/select-process/functions.h"
    #include "application/user-interface/windows/change-value/functions.h"

    #include "application/user-interface/windows/memory-scanner/window.h"
    #include "application/user-interface/windows/select-process/window.h"
    #include "application/user-interface/windows/change-value/window.h"

    #include "application/user-interface/windows/memory-scanner/window-procedure-message-handlers.h"
    #include "application/user-interface/windows/select-process/window-procedure-message-handlers.h"
    #include "application/user-interface/windows/change-value/window-procedure-message-handlers.h"

    #include "application/user-interface/windows/memory-scanner/window-procedure.h"
    #include "application/user-interface/windows/select-process/window-procedure.h"
    #include "application/user-interface/windows/change-value/window-procedure.h"

/* End GUI code import */

/****************************************************************************************************************** 
 ******************************************************************************************************************/

/* Initialize global variables. */

boolean Initialize(HINSTANCE application_instance)
{
    FirstScanNotRun = true;
    SelectedItem = -1;
    ScreenDPI = 96;
    
    Instance = (application_instance) ? application_instance : GetModuleHandleA(null);

    CommonControls.dwICC = ICC_WIN95_CLASSES;
    CommonControls.dwSize = sizeof(CommonControls);

    return InitCommonControlsEx(&CommonControls);
}

#endif
