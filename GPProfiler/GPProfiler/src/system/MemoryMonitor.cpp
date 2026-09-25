#include "MemoryMonitor.h"

#include <Windows.h>

MemorySnapshot MemoryMonitor::GetSnapshot() const
{
    MEMORYSTATUSEX memoryStatus;

    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

    if (!GlobalMemoryStatusEx(&memoryStatus))
    {
        return {};
    }

    const auto totalMemory =
        memoryStatus.ullTotalPhys;

    const auto availableMemory =
        memoryStatus.ullAvailPhys;

    const auto usedMemory =
        totalMemory - availableMemory;

    double usagePercentage = 0.0;

    if (totalMemory > 0)
    {
        usagePercentage =
            (static_cast<double>(usedMemory) /
                static_cast<double>(totalMemory)) * 100.0;
    }

    return
    {
        totalMemory,
        availableMemory,
        usedMemory,
        usagePercentage
    };
}