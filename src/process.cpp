#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() 
{ 
    return pid; 
}

float Process::CpuUtilization() 
{
     UpdateParams();
     float hrtz = sysconf(_SC_CLK_TCK);

     float total_time = utime + stime;
     total_time = total_time + cutime + cstime;

     float seconds = uptime - (starttime / hrtz);

     cpu_usage = ((total_time / hrtz) / seconds);
     return cpu_usage <= 0 ? 0 : cpu_usage; 
}

void Process::UpdateParams()
{
   auto params = LinuxParser::ProcessCpuUtilization(pid);

    uptime = std::stof(params[0]);
    utime = std::stof(params[1]);
    stime = std::stof(params[2]);
    cutime = std::stof(params[3]);
    cstime = std::stof(params[4]);
    starttime = std::stof(params[5]);
}

string Process::Command() 
{ 
    return LinuxParser::Command(pid); 
}

string Process::Ram() 
{ 
    int memory = std::stof(LinuxParser::Ram(pid));
    int memoryMb = memory/1024;
    return std::to_string(memoryMb);
}

string Process::User() 
{ 
    string uid = LinuxParser::Uid(pid);
    return LinuxParser::User(std::stoi(uid));
}

long int Process::UpTime() 
{ 
    long utime = LinuxParser::UpTime(pid);
    return utime/sysconf(_SC_CLK_TCK);
}

bool Process::operator<(Process const& a) const 
{
    if(cpu_usage < a.cpu_usage)
        return true;
    else
        return false;
}