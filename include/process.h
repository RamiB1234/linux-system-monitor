#ifndef PROCESS_H
#define PROCESS_H

#include <string>


class Process {
 public:
  Process(int pid): pid(pid) {};  
  int Pid();                         
  std::string User();                   
  std::string Command();                 
  float CpuUtilization();                
  std::string Ram();                     
  long int UpTime();                       
  bool operator<(Process const& a) const;  

 private:
   int pid;
   void UpdateParams();
   float cpu_usage{0}, uptime{0}, utime{0}, stime{0}, cutime{0}, cstime{0}, starttime{0};
};

#endif