#ifndef _SELECT_PROCESS_WINDOW_PROCEDURE_MESSAGE_HANDLERS_H
#define _SELECT_PROCESS_WINDOW_PROCEDURE_MESSAGE_HANDLERS_H

/* Runs when a selection is made or changed in listbox containing process names on process selection dialog. */

void ProcessListboxChangeEvent(void)
{
    IndexOfSelectedProcess = (int32)SendMessageA(ProcessSelection, LB_GETCURSEL, 0, 0);

    if(IndexOfSelectedProcess > -1)
    {
        int8 selected_process[256];

        ZeroMemory(&selected_process, sizeof(selected_process));
        CopyString(selected_process, Processes[IndexOfSelectedProcess], sizeof(selected_process) - 1);

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
    uint16 index;

    if(IndexOfSelectedProcess > -1)
    {
        int8 pid[256];
        HANDLE process;
        uint32 process_id;
        int8 selected_process[256];

        ZeroMemory(&pid, sizeof(pid));
        ZeroMemory(&selected_process, sizeof(selected_process));
        
        CopyString(pid, Pids[IndexOfSelectedProcess], sizeof(pid) - 1);
        CopyString(SelectedPid, Pids[IndexOfSelectedProcess], sizeof(SelectedPid) - 1);
        CopyString(selected_process, Processes[IndexOfSelectedProcess], sizeof(selected_process) - 1);

        process_id = (uint32)StringToInteger(pid, FMT_INT_DECIMAL);
        process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, process_id);

        if(process)
        {
            int8 data_size[256];
            LRESULT selection_id = SendMessage(DataSize, CB_GETCURSEL, 0, 0);

            if(selection_id > -1) 
            {
                ZeroMemory(data_size, sizeof(data_size));
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

                if(ArrayList)
                {
                    FreeArrayList(ArrayList);
                    ArrayList = CreateArrayList(sizeof(*Scanner));
                }

                Scanner = CreateMemoryScanner(CurrentProcess, (uint16)StringToInteger(data_size, FMT_INT_DECIMAL));

                if(MonitorSelectedProcessThread && TerminateThread(MonitorSelectedProcessThread, 0))
                {
                    WaitForSingleObject(MonitorSelectedProcessThread, INFINITE); 
                    CloseHandle(MonitorSelectedProcessThread);
                }

                MonitorSelectedProcessThread = CreateThread(null, null, (LPTHREAD_START_ROUTINE)MonitorSelectedProcess, null, null, null);
            }

            if(StringLength(selected_process))
            {          
                SetWindowText(ChoosePid, Processes[IndexOfSelectedProcess]);
            }

            else
            {
                ResetScan(Scanner, true, true);
            }

            EnableWindow(SearchCondition, true);
            EnableWindow(DataSize, true);
            EnableWindow(NewScan, false);
            EnableWindow(Value, true);
            EnableWindow(Scan, true);
            CloseHandle(process);
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
    }
    
    SendMessageA(DataSize, CB_RESETCONTENT, 0, 0);

    for(index = 0; index < ARRAYSIZE(DataTypes); index++)
    {
        SendMessageA(DataSize, CB_ADDSTRING, 0, (LPARAM)DataTypes[index]);
    }

    SendMessageA(SearchCondition, CB_SETCURSEL, (WPARAM)0, 0);
    SendMessageA(DataSize, CB_SETCURSEL, (WPARAM)2, 0);

    SetForegroundWindow(MemoryScannerWindow);
    EnableWindow(MemoryScannerWindow, true);
}

#endif
