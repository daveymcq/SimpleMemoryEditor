#ifndef _SELECT_PROCESS_WINDOW_EVENT_HANDLERS_H
#define _SELECT_PROCESS_WINDOW_EVENT_HANDLERS_H

/* Runs when a selection is made or changed in listbox containing process names on process selection dialog. */

void ProcessListboxChangeEvent(void)
{
    IndexOfSelectedProcess = (int32)SendMessageA(ProcessSelection, LB_GETCURSEL, 0, 0);

    if(IndexOfSelectedProcess > -1)
    {
        int8 selected_process[256] = { 0 };

        CopyString(selected_process, Pids[IndexOfSelectedProcess], sizeof(selected_process) - 1);

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

/* Runs when "Select Process" button is clicked. */

void ProcessSelectProcessButtonEvent(void)
{
    if(IndexOfSelectedProcess > -1)
    {
        int8 selected_process[256];
        int8 pid[256];

        HANDLE process;
        uint32 process_id;

        CopyString(selected_process, Pids[IndexOfSelectedProcess], sizeof(selected_process) - 1);

        if(StringLength(selected_process))
        {
            CopyString(pid, Pids[IndexOfSelectedProcess], sizeof(pid) - 1);
            CopyString(SelectedPid, pid, sizeof(SelectedPid) - 1);
            SendMessage(Pid, WM_SETTEXT, 0, (LPARAM)Processes[IndexOfSelectedProcess]);
        }

        else
        {
            ResetScan(Scanner, true, true);
        }

        process_id = (uint32)StringToInteger(pid, FMT_INT_DECIMAL);
        process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, process_id);

        if(process)
        {
          
            int8 data_size[256];
            LRESULT selection_id = SendMessage(DataSize, CB_GETCURSEL, 0, 0);

            if(selection_id > -1) 
            {
                CopyString(data_size, (string)DataSizes[selection_id], sizeof(data_size) - 1);
            }

            if(CurrentProcess != process_id)
            {
                CurrentProcess = process_id;

                if(Scanner)
                {
                    ResetScan(Scanner, false, true);
                    FreeMemoryScanner(Scanner);
                }

                Scanner = CreateMemoryScanner(CurrentProcess, (uint16)StringToInteger(data_size, FMT_INT_DECIMAL));

                if(MonitorSelectedProcessThread && TerminateThread(MonitorSelectedProcessThread, 0))
                {
                    WaitForSingleObject(MonitorSelectedProcessThread, INFINITE); 
                    CloseHandle(MonitorSelectedProcessThread);
                }

                MonitorSelectedProcessThread = CreateThread(null, null, (LPTHREAD_START_ROUTINE)MonitorSelectedProcess, null, null, null);
            }
        

            CloseHandle(process);
            EnableWindow(Scan, true);
        }

        else
        {
            if(Scanner) 
            {
                ResetScan(Scanner, true, true);
            }

            EnableWindow(Scan, false);
            EnableWindow(ChoosePid, true);
        }

        DestroyWindow(SelectPidWindow);
        EnableWindow(MainWindow, true);
        SetForegroundWindow(MainWindow);
    }
}

#endif
