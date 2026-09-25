#include <iostream>
#include <thread>
#include <chrono>

#include "system/CpuMonitor.h"
#include "system/MemoryMonitor.h"
#include "system/ProcessMonitor.h"



double BytesToGB(unsigned long long bytes)
{
    return static_cast<double>(bytes) /
        (1024.0 * 1024.0 * 1024.0);
}

double BytesToMB(unsigned long long bytes)
{
    return static_cast<double>(bytes) /
        (1024.0 * 1024.0);
}


int main()
{
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;

    std::vector<ProcessInfo> processes = processMonitor.GetProcesses();

    for (const ProcessInfo& process : processes)
    {
        std::cout
            << "PID: "
            << process.processId
            << " | "
            << process.name
            << " | RAM: "
            << std::fixed
            << std::setprecision(2)
            << BytesToMB(process.memoryUsage)
            << " MB\n";
    }
    
    /*
    while (true)
    {
        double cpuUsage = cpuMonitor.GetUsage();
        MemorySnapshot memory = memoryMonitor.GetSnapshot();

        std::cout << "\rCPU: "
            << std::fixed
            << std::setprecision(1)
            << cpuUsage
            << "% | RAM: "
            << memory.usagePercentage
            << "% | "
            << std::setprecision(2)
            << BytesToGB(memory.usedMemory)
            << " / "
            << BytesToGB(memory.totalMemory)
            << " GB   "
            << std::flush;

        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );
    }
    */
    return 0;
}