#pragma once

class MemoryMonitor
{
public:
    double GetUsagePercentage() const;

    unsigned long long GetTotalMemory() const;
    unsigned long long GetAvailableMemory() const;
    unsigned long long GetUsedMemory() const;

    double GetTotalMemoryGB() const;
    double GetUsedMemoryGB() const;
};