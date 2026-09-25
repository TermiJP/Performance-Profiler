#pragma once


struct MemorySnapshot 
{
    unsigned long long totalMemory;
    unsigned long long availableMemory;
    unsigned long long usedMemory;
    double usagePercentage;
};

class MemoryMonitor
{
public:
    MemorySnapshot GetSnapshot() const;
};