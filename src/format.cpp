#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) 
{ 
    int hours, minutes;
    minutes = seconds / 60;
    hours = minutes / 60;
    string time = std::to_string(int(hours)) + ":" + std::to_string(int(minutes%60)) + ":"+ std::to_string(int(seconds%60));

    return time; 
}