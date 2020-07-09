#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
public:
    Process(int pid);
    
    /*
    Returns the process id number
    */
    int Pid();

    /*
    Returns the user name that generated this process
    */                               
    std::string User();

    /*
    Returns the command that generated this process
    */                      
    std::string Command();

    /*
    Returns the CPU utilization for this process
    */                   
    float CpuUtilization();

    /*
    Returns the memory utilization for this process
    */                  
    std::string Ram();                       

    /*
    Returns the uptime of the process in seconds 
    */
    long int UpTime();

    /*
    Operator overloading: < - Process Object
    */                       
    bool operator<(Process const& a) const;  


private:
    int _pid{0};
    std::string _usr{}, _cmd{}, _ram{};
    float _seconds{0.0}, _total_time{0.0}, _cpu_util{0.0};
};

#endif