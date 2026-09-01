/* Memory Editing Application
 Author: David McHugh Jr.
 Last Modified: 09/01/2026 */

#include "initialize.h"

/* Entry point. */

void main(int32 argc, string argv[])
{
    if(Initialize())
    {
        if(CreateMemoryScannerWindow())
        {
            while(GetMessage(&message, null, 0, 0) > 0)
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
    }
}
