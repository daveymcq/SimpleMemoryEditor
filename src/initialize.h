#ifndef _GLOBAL_DECLARATIONS_H
#define _GLOBAL_DECLARATIONS_H

#include "../resources/icon.h"

#include "lib/qlib/QLIB/qlib.h"

#include "functions/gui-functions/includes.h"
#include "functions/memory-scanner-functions/includes.h"

#include "functions/memory-scanner-functions/functions.h"
#include "functions/gui-functions/functions.h"

// Initialize local variables.

BOOL Initialize(void)
{
    INITCOMMONCONTROLSEX icc;

    Width = 625;
    Height = 405;
    FirstScanNotRun = true;
    SelectedItem = -1;

    icc.dwICC = ICC_WIN95_CLASSES;
    icc.dwSize = sizeof(icc);

    return InitCommonControlsEx(&icc);
}

#endif
