#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

// Initializing static data members for Processor class
float Processor::_total_jiffies_prev{0};
float Processor::_util_jiffies_prev{0};
 
float Processor::Utilization() { 
    
    std::vector<std::string> jiffies_list = LinuxParser::CpuUtilization();
    for (size_t i=0; i<jiffies_list.size(); ++i) {
        if (i!=LinuxParser::kGuest_ and i!=LinuxParser::kGuestNice_) {
            _total_jiffies += std::stof(jiffies_list[i]); 
        }
        if (i == LinuxParser::kIdle_ or i == LinuxParser::kIOwait_) {
            _idle_jiffies += std::stof(jiffies_list[i]);
        }
    }
    
    _util_jiffies = _total_jiffies - _idle_jiffies;

    _usage = (_util_jiffies - _util_jiffies_prev)/(_total_jiffies-_total_jiffies_prev);
    _total_jiffies_prev = _total_jiffies;
    _util_jiffies_prev = _util_jiffies;
    
    return _usage;
}