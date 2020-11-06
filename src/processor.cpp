#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() 
{ 
    float prevIdle = idle +iowait;
    float prevNonIdle = user + nice + system + irq + softirq + steal;
    float prevTotal = prevIdle + prevNonIdle;

    Processor::UpdateParams();

    float newIdle = idle +iowait;
    float newNonIdle = user + nice + system + irq + softirq + steal;
    float newTotal = newIdle + newNonIdle;

    float totald = newTotal - prevTotal;
    float idled = newIdle - prevIdle;

    float percentage = (totald - idled)/totald;
    return percentage;

}

void Processor::UpdateParams()
{
   auto params = LinuxParser::CpuUtilization();

    user = std::stof(params[0]);
    nice = std::stof(params[1]);
    system = std::stof(params[2]);
    idle = std::stof(params[3]);
    iowait = std::stof(params[4]);
    irq = std::stof(params[5]);
    softirq = std::stof(params[6]);
    steal = std::stof(params[7]);
    guest = std::stof(params[8]);
    guest_nice = std::stof(params[9]);
}