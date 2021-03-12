#ifndef _GLOBAL_DECLARATIONS_H
#define _GLOBAL_DECLARATIONS_H

#include "../resources/icon.h"

#include "lib/qlib/QLIB/qlib.h"

#include "functions/gui/includes.h"
#include "functions/memory-Scanner/includes.h"

#include "functions/memory-Scanner/functions.h"
#include "functions/gui/functions.h"

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
