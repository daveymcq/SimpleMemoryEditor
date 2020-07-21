#ifndef _GLOBAL_DECLARATIONS_H
#define _GLOBAL_DECLARATIONS_H

#include "gui/gui-declarations.h"
#include "scanner/scanner-declarations.h"

// Initialize local variables.

BOOL Initialize(void)
{
    Width = 625;
    Height = 425;
    FirstScanNotRun = true;
    SelectedItem = -1;

    INITCOMMONCONTROLSEX icc;
    icc.dwICC = ICC_WIN95_CLASSES;
    icc.dwSize = sizeof(icc);

    return InitCommonControlsEx(&icc);
}

#endif
