/* Memory Editing Application
 Author: David McHugh Jr.
 Last Modified: 07/12/2025 */

#include "initialize.h"

/* Entry point. */

void WINAPI Main(int32 argc, string argv[])
{
    if(Initialize())
    {
        if(CreateMemoryScannerWindow())
        {
            MSG message;
            while(GetMessage(&message, null, 0, 0) > 0)
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
    }
}
