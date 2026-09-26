#include "ProcessMonitor.h"

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

static unsigned long long FileTimeToUInt64(const FILETIME& fileTime)
{
    ULARGE_INTEGER value;

    value.LowPart = fileTime.dwLowDateTime;
    value.HighPart = fileTime.dwHighDateTime;

    return value.QuadPart;
}


std::vector<ProcessInfo> ProcessMonitor::GetProcesses() 
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
            process.cpuUsage = 0.0;

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

                FILETIME creationTime;
                FILETIME exitTime;
                FILETIME kernelTime;
                FILETIME userTime;

                if (GetProcessTimes(
                    processHandle,
                    &creationTime,
                    &exitTime,
                    &kernelTime,
                    &userTime))
                {
                    const unsigned long long currentKernelTime =
                        FileTimeToUInt64(kernelTime);

                    const unsigned long long currentUserTime =
                        FileTimeToUInt64(userTime);

                    const auto iterator =
                        previousTimes.find(
                            process.processId
                        );

                    if (iterator != previousTimes.end())
                    {
                        const unsigned long long kernelDelta =
                            currentKernelTime -
                            iterator->second.kernelTime;

                        const unsigned long long userDelta =
                            currentUserTime -
                            iterator->second.userTime;

                        const unsigned long long processDelta =
                            kernelDelta + userDelta;

                        FILETIME systemIdleTime;
                        FILETIME systemKernelTime;
                        FILETIME systemUserTime;

                        if (GetSystemTimes(
                            &systemIdleTime,
                            &systemKernelTime,
                            &systemUserTime))
                        {
                            const auto currentSystemKernel =
                                FileTimeToUInt64(
                                    systemKernelTime);

                            const auto currentSystemUser =
                                FileTimeToUInt64(
                                    systemUserTime);

                            const auto systemKernelDelta =
                                currentSystemKernel -
                                previousSystemKernelTime;

                            const auto systemUserDelta =
                                currentSystemUser -
                                previousSystemUserTime;

                            const auto systemDelta =
                                systemKernelDelta +
                                systemUserDelta;

                            if (systemDelta > 0)
                            {
                                process.cpuUsage =
                                    (static_cast<double>(
                                        processDelta) /
                                        static_cast<double>(
                                            systemDelta)) *
                                    100.0;
                            }
                        }
                    }

                    ProcessTime currentTime;

                    currentTime.kernelTime = currentKernelTime;
                    currentTime.userTime = currentUserTime;

                    previousTimes[process.processId] = currentTime;
                }

                CloseHandle(processHandle);
            }

            processes.push_back(process);

        } while (Process32Next( snapshot,&processEntry));
    }

    CloseHandle(snapshot);

    return processes;
}