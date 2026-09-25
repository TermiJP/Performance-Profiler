#include <iostream>
#include <thread>
#include <chrono>

#include "system/CpuMonitor.h"
#include "system/MemoryMonitor.h"


int main()
{
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;

    double usedMemoryGB =
        memoryMonitor.GetUsedMemoryGB();

    double totalMemoryGB =
        memoryMonitor.GetTotalMemoryGB();

    while (true)
    {
        double cpuUsage = cpuMonitor.GetUsage();
        double memoryUsage = memoryMonitor.GetUsagePercentage();

        std::cout << "\rCPU: "
            << std::fixed
            << std::setprecision(1)
            << cpuUsage
            << "% | RAM: "
            << memoryUsage
            << "% | "
            << std::setprecision(2)
            << usedMemoryGB
            << " / "
            << totalMemoryGB
            << " GB   "
            << std::flush;

        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );
    }

    return 0;
}