
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() 
{ 
  string line;
  string key;
  string value;
  float totalMemory =0;
  float freeMemory =0;
  float usedMemory =0;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          totalMemory = std::stof(value);
        }
        else if (key == "MemFree") {
          freeMemory = std::stof(value);
        }
      }
    }
  }
  usedMemory = totalMemory - freeMemory;
  float percentage = usedMemory / totalMemory;
  return percentage;
}

long LinuxParser::UpTime() 
{ 
  string uptime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return std::stol(uptime)+std::stol(idletime);
}

vector<string> LinuxParser::CpuUtilization() 
{ 
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  return vector<string> {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
}

int LinuxParser::TotalProcesses() 
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return std::stoi(value);
}

int LinuxParser::RunningProcesses() 
{ 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return std::stoi(value);
}

string LinuxParser::Command(int pid) 
{ 
  string command;
  string line;
   string path = kProcDirectory+std::to_string(pid)+kCmdlineFilename;
  std::ifstream stream(path);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;
  }
  return command; 
}

string LinuxParser::Ram(int pid) 
{
  string line;
  string key;
  string value;
  string path = kProcDirectory+std::to_string(pid)+kStatusFilename;
  std::ifstream filestream(path);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Uid(int pid) 
{ 
  string line;
  string key;
  string value;
  string path = kProcDirectory+std::to_string(pid)+kStatusFilename;
  std::ifstream filestream(path);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          return value;
        }
      }
    }
  }
  return value;
}


string LinuxParser::User(int uid) 
{ 
  string line;
  string key;
  string value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
            std::replace(line.begin(), line.end(), 'x', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (value == std::to_string(uid)) {
          return key;
        }
      }
    }
  }
  return value;
}

vector<string> LinuxParser::ProcessCpuUtilization(int pid) 
{ 
  string uptime, utime, stime, cutime, cstime, starttime;
  string x;
  string line;
  string path = kProcDirectory+std::to_string(pid)+kStatFilename;
  std::ifstream stream(path);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> x >> x >> x >> x >> x >> x >> x >> x >> x >> x >> x >> x
    >> utime >> stime >> cutime >> cstime >> x >> x >> x >> x >> starttime;
  }
  return vector<string> {uptime, utime, stime, cutime, cstime, starttime};
}

long LinuxParser::UpTime(int pid) 
{ 
  string uptime;
  string x;
  string line;
  string path = kProcDirectory+std::to_string(pid)+kStatFilename;
  std::ifstream stream(path);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> x >> x >> x >> x >> x >> x >> x >> x >> x >> x >> x >> x >> x
    >> x >> x >> x >> x >> x >> x >> x >> x >> uptime;
  }
  return std::stol(uptime); 
}