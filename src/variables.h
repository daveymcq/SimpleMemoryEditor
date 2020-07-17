#ifndef _VARIABLES_H
#define _VARIABLES_H

#include "gui/functions/variables.h"
#include "scanner/functions/variables.h"

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
