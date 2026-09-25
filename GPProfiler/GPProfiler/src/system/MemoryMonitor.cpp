#include "MemoryMonitor.h"

#include <Windows.h>

double MemoryMonitor::GetUsagePercentage() const
{
    MEMORYSTATUSEX memoryStatus;

    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

    if (!GlobalMemoryStatusEx(&memoryStatus))
    {
        return 0.0;
    }

    const double totalMemory =
        static_cast<double>(memoryStatus.ullTotalPhys);

    const double availableMemory =
        static_cast<double>(memoryStatus.ullAvailPhys);

    if (totalMemory == 0.0)
    {
        return 0.0;
    }

    const double usedMemory =
        totalMemory - availableMemory;

    return (usedMemory / totalMemory) * 100.0;
}

unsigned long long MemoryMonitor::GetTotalMemory() const
{
    MEMORYSTATUSEX memoryStatus;

    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

    if (!GlobalMemoryStatusEx(&memoryStatus))
    {
        return 0;
    }

    return memoryStatus.ullTotalPhys;
}

unsigned long long MemoryMonitor::GetAvailableMemory() const
{
    MEMORYSTATUSEX memoryStatus;

    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

    if (!GlobalMemoryStatusEx(&memoryStatus))
    {
        return 0;
    }

    return memoryStatus.ullAvailPhys;
}

unsigned long long MemoryMonitor::GetUsedMemory() const
{
    const auto totalMemory = GetTotalMemory();
    const auto availableMemory = GetAvailableMemory();

    if (totalMemory < availableMemory)
    {
        return 0;
    }

    return totalMemory - availableMemory;
}