#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System

/*
Returns the memory utilization for the system
*/
float MemoryUtilization();

/*
Returns the uptime in seconds for the system 
*/
long UpTime();

/*
Returns the process IDs for processes running on the system 
*/
std::vector<int> Pids();

/*
Returns the total number of processes on the system
*/
int TotalProcesses();

/*
Returns the running processes on the system
*/
int RunningProcesses();

/*
Returns the Operating System 
*/
std::string OperatingSystem();

/*
Returns the Kernel for the current operating system
*/
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

/*
Returns the CPU utilization for the system
*/
std::vector<std::string> CpuUtilization();


long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif