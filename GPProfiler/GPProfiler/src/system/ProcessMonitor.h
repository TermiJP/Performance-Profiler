#pragma once

#include <string>
#include <vector>

#include <unordered_map>

struct ProcessInfo
{
    unsigned long processId;
    std::string name;

    unsigned long long memoryUsage;
    double cpuUsage;
};

class ProcessMonitor
{
public:
    std::vector<ProcessInfo> GetProcesses();

private:
    struct ProcessTime 
    {
        unsigned long long kernelTime;
        unsigned long long userTime;
        
    };

    std::unordered_map<unsigned long, ProcessTime> previousTimes;

    unsigned long long previousSystemKernelTime = 0;
    unsigned long long previousSystemUserTime = 0;
};