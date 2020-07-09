#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"


Process::Process(int pid) 
: _pid(pid) {
    _cpu_util = Process::CpuUtilization();
    _cmd = LinuxParser::Command(_pid);

}

int Process::Pid() { 
    return _pid; 
}

float Process::CpuUtilization() { 
    long int frequency = sysconf(_SC_CLK_TCK);
    _total_time = LinuxParser::ActiveJiffies(_pid)/frequency;
    _seconds = LinuxParser::UpTime(_pid);
    return _total_time/_seconds;
}

std::string Process::Command() { 
    return _cmd; 
}

std::string Process::Ram() { 
    return LinuxParser::Ram(_pid); 
}

std::string Process::User() { 
    return LinuxParser::User(_pid); 
}

long int Process::UpTime() { 
    return LinuxParser::UpTime(_pid); 
}

bool Process::operator<(Process const& a) const { 
    return _cpu_util > a._cpu_util; 
}