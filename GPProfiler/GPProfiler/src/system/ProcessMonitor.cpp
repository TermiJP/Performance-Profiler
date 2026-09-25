#include "ProcessMonitor.h"

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

std::vector<ProcessInfo> ProcessMonitor::GetProcesses() const
{
    std::vector<ProcessInfo> processes;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
       

    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return processes;
    }

    PROCESSENTRY32 processEntry;

    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First( snapshot, &processEntry))
    {
        do
        {
            ProcessInfo process;

            process.processId =
                processEntry.th32ProcessID;

            process.name =
                processEntry.szExeFile;

            process.memoryUsage = 0;

            HANDLE processHandle = OpenProcess(
                    PROCESS_QUERY_INFORMATION |
                    PROCESS_VM_READ,
                    FALSE,
                    process.processId
                );

            if (processHandle != nullptr)
            {
                PROCESS_MEMORY_COUNTERS memoryCounters;

                if (GetProcessMemoryInfo(
                    processHandle,
                    &memoryCounters,
                    sizeof(memoryCounters)))
                {
                    process.memoryUsage =
                        memoryCounters.WorkingSetSize;
                }

                CloseHandle(processHandle);
            }

            processes.push_back(process);

        } while (Process32Next( snapshot,&processEntry));
    }

    CloseHandle(snapshot);

    return processes;
}