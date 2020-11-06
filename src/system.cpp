#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>  

#include "process.h"
#include "processor.h"
#include "system.h"

#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() 
{ return cpu_; }

bool CompareProcess(Process p1, Process p2) 
{ 
    return (p2 < p1); 
} 

vector<Process>& System::Processes() 
{
     auto processes = LinuxParser::Pids();
     for(auto pid : processes)
     {
         Process p(pid);
         processes_.emplace_back(p);
     }

     std::sort(processes_.begin(), processes_.end(), CompareProcess);

     return processes_; 
}

std::string System::Kernel() 
{ 
    return LinuxParser::Kernel(); 
}

float System::MemoryUtilization() 
{ 
    return LinuxParser::MemoryUtilization(); 
}

std::string System::OperatingSystem() 
{ 
    return LinuxParser::OperatingSystem();
}

int System::RunningProcesses() 
{ 
    return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() 
{ 
    return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() 
{ 
    return LinuxParser::UpTime();
}