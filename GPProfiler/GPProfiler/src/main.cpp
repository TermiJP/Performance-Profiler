#include <iostream>
#include <thread>
#include <chrono>

#include "system/CpuMonitor.h"
#include "system/MemoryMonitor.h"


int main()
{
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;

    while (true)
    {
        double cpuUsage = cpuMonitor.GetUsage();
        double memoryUsage = memoryMonitor.GetUsagePercentage();

        std::cout << "\rCPU: " << cpuUsage << "% | RAM: " << memoryUsage << "%  "
            << std::flush;

        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );
    }

    return 0;
}