#ifndef _GLOBAL_DECLARATIONS_H
#define _GLOBAL_DECLARATIONS_H

#include "gui/gui-declarations.h"
#include "scanner/scanner-declarations.h"

// Initialize local variables.

BOOL Initialize(void)
{
    MainWindowWidth = 619;
    MainWindowHeight = 410;

    ChooseProcessWindowWidth = 290;
    ChooseProcessWindowHeight = 405;

    ChangeValueWindowWidth = 295;
    ChangeValueWindowHeight = 80;

    FirstScanNotRun = true;
    SelectedItem = -1;

    INITCOMMONCONTROLSEX icc;
    icc.dwICC = ICC_WIN95_CLASSES;
    icc.dwSize = sizeof(icc);

    return InitCommonControlsEx(&icc);
}

#endif
