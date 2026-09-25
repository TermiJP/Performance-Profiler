#include "CpuMonitor.h"
#include <Windows.h>

static unsigned long long FileTimeToUInt64(const FILETIME& fileTime)
{
    ULARGE_INTEGER value;

    value.LowPart = fileTime.dwLowDateTime;
    value.HighPart = fileTime.dwHighDateTime;

    return value.QuadPart;
}

CpuMonitor::CpuMonitor()
    : previousIdleTime(0),
    previousKernelTime(0),
    previousUserTime(0)
{
}

double CpuMonitor::GetUsage()
{
    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;

    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime))
    {
        return 0.0;
    }

    const auto currentIdle =
        FileTimeToUInt64(idleTime);

    const auto currentKernel =
        FileTimeToUInt64(kernelTime);

    const auto currentUser =
        FileTimeToUInt64(userTime);

    // First measurement: we don't have a previous
    // measurement to compare against yet.
    if (previousKernelTime == 0 &&
        previousUserTime == 0)
    {
        previousIdleTime = currentIdle;
        previousKernelTime = currentKernel;
        previousUserTime = currentUser;

        return 0.0;
    }

    const auto idleDelta =
        currentIdle - previousIdleTime;

    const auto kernelDelta =
        currentKernel - previousKernelTime;

    const auto userDelta =
        currentUser - previousUserTime;

    // Store the current values for the next measurement.
    previousIdleTime = currentIdle;
    previousKernelTime = currentKernel;
    previousUserTime = currentUser;

    const auto totalDelta =
        kernelDelta + userDelta;

    if (totalDelta == 0)
    {
        return 0.0;
    }

    const double idleTimePercentage =
        static_cast<double>(idleDelta) /
        static_cast<double>(totalDelta);

    const double cpuUsage =
        100.0 * (1.0 - idleTimePercentage);

    return cpuUsage;
}

