#ifndef _SELECT_PROCESS_WINDOW_FUNCTIONS_H
#define _SELECT_PROCESS_WINDOW_FUNCTIONS_H

/* Finds all running processes on machine and finds their process id. */

boolean GetProcessNameAndID(VOID)
{
    HANDLE snapshot, process;
    PROCESSENTRY32 pe;

    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if(snapshot != INVALID_HANDLE_VALUE)
    {
        pe.dwSize = sizeof(pe);

        if(!Process32First(snapshot, &pe))
        {
            CloseHandle(snapshot);
            return false;
        }

        do
        {
            if(pe.th32ProcessID == GetCurrentProcessId()) 
            {
                continue;
            }

            process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, pe.th32ProcessID); 

            if(process)
            {
                boolean IsProcess32Bit;
                boolean IsApplication32Bit;

                typedef boolean (WINAPI *FP_ISWOW64PROCESS) (HANDLE, PBOOL);
                FP_ISWOW64PROCESS pIsWow64Process = (FP_ISWOW64PROCESS)GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process");

                CopyString(Processes[NumberOfProcesses], pe.szExeFile, sizeof(Processes[NumberOfProcesses]));

                if(pIsWow64Process)
                {
                    if(pIsWow64Process(GetCurrentProcess(), &IsApplication32Bit))
                    {
                        if(pIsWow64Process(process, &IsProcess32Bit))
                        {
                            if(IsApplication32Bit)
                            {
                                if(!IsProcess32Bit)
                                {
                                    continue;
                                }
                            }
                        }
                    }
                }
                
                IntegerToString(pe.th32ProcessID, Pids[NumberOfProcesses], sizeof(Pids[NumberOfProcesses]) - 1, FMT_INT_DECIMAL);
                NumberOfProcesses++; 
               
                CloseHandle(process);
            }

        } while(Process32Next(snapshot, &pe));

        ProcessCounter = NumberOfProcesses;
        NumberOfProcesses = 0;

        CloseHandle(snapshot);

        return true;
    }

    return false;
}

#endif
