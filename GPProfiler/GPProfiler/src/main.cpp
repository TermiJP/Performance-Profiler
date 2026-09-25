#include <iostream>
#include <thread>
#include <chrono>

#include "system/CpuMonitor.h"

int main()
{
    CpuMonitor cpuMonitor;

    while (true)
    {
        double cpuUsage = cpuMonitor.GetUsage();

        std::cout << "\rCPU Usage: "
            << cpuUsage
            << "%   "
            << std::flush;

        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );
    }

    return 0;
}