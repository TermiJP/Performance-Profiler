#pragma once

#include <string>
#include <vector>

struct ProcessInfo
{
    unsigned long processId;
    std::string name;

    unsigned long long memoryUsage;
};

class ProcessMonitor
{
public:
    std::vector<ProcessInfo> GetProcesses() const;
};