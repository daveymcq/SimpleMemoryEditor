#ifndef _SELECT_PROCESS_WINDOW_EVENT_HANDLERS_H
#define _SELECT_PROCESS_WINDOW_EVENT_HANDLERS_H

// Runs when a selection is made or changed in listbox containing process names on process selection dialog.

void ProcessListboxChangeEvent(void)
{
    IndexOfSelectedProcess = (int)SendMessage(ProcessSelection, LB_GETCURSEL, 0, 0);

    if(IndexOfSelectedProcess > -1)
    {
        char selected_process[256];

        CopyString(selected_process, pids[IndexOfSelectedProcess], sizeof(selected_process));

        if(StringLength(selected_process))
        {
            EnableWindow(ChooseProcess, true);
        }

        else
        {
            EnableWindow(ChooseProcess, false);
        }
    }
}

#endif
