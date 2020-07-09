#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

std::string LinuxParser::OperatingSystem() {
    std::string line, key, value;
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

std::string LinuxParser::Kernel() {
    std::string os, version, kernel;
    std::string line;
    std::ifstream stream(kProcDirectory + kVersionFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> version >> kernel;  
    }
    return kernel;
}

std::vector<int> LinuxParser::Pids() {
    std::vector<int> pids;
    DIR* directory = opendir(kProcDirectory.c_str());
    struct dirent* file;
    while ((file = readdir(directory)) != nullptr) {
        // Is this a directory?
        if (file->d_type == DT_DIR) {
            // Is every character of the name a digit?
            std::string filename(file->d_name);
            if (std::all_of(filename.begin(), filename.end(), isdigit)) {
                int pid = stoi(filename);
                pids.push_back(pid);
            }
        }
    }
    closedir(directory);
    return pids;
}

float LinuxParser::MemoryUtilization() { 
    float mem_total, mem_free;
    std::string line, key, value;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open()){
        std::getline(stream, line);
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
            if (key == "MemTotal:") 
                mem_total = std::stof(value);
            if (key == "MemFree:") 
                mem_free = std::stof(value);
        }    
    }

    return (mem_total-mem_free)/mem_total; 
}

long LinuxParser::UpTime() { 
    long uptime_long{0};
    std::string line, uptime_string, idletime_string;
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()){
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> uptime_string >> idletime_string; 
        uptime_long = std::stol(uptime_string);
    }
    
    return uptime_long; 
}


long LinuxParser::Jiffies() { return 0; }

long LinuxParser::ActiveJiffies(int pid) { 
    std::string jiffies_str, line;
    long total_clk;
    std::vector<std::string> stat_list;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        while (linestream >> jiffies_str) {
            stat_list.push_back(jiffies_str);
        }
    }

    long int utime{std::stol(stat_list[13])};
    long int stime{std::stol(stat_list[14])};
    long int cutime{std::stol(stat_list[15])};
    long int cstime{std::stol(stat_list[16])};

    total_clk = utime + stime + cutime + cstime;
    
    return total_clk; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }


std::vector<std::string> LinuxParser::CpuUtilization() { 
    std::string key, value, line;
    std::vector<std::string> jiffies_list;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        while(linestream >> key) {
            if(key != "cpu") {
                jiffies_list.push_back(key);
            }
        }
    } 
    return jiffies_list;
}

int LinuxParser::TotalProcesses() { 
    int total_processes;
    std::string key, value, line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
            if (key == "processes") {
                total_processes = std::stoi(value);
                return total_processes;
            }
        }
    } 
    return 12; 
}

int LinuxParser::RunningProcesses() { 
    int running_processes; 
    std::string key, value, line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "procs_running") {
                    running_processes = std::stoi(value);
                    return running_processes;
                }
            }
        }
    }
    return 0; 
}

std::string LinuxParser::Command(int pid) { 
    std::string cmd, line;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            std::replace(line.begin(), line.end(), ' ', '*');
            while (linestream >> cmd) {
                std::replace(cmd.begin(), cmd.end(), '*', ' ');
                return cmd;
            }
        }
    }
    return std::string(); 
}

std::string LinuxParser::Ram(int pid[[maybe_unused]]) { 
    std::string key, value, kb, line;
    int vmsize;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value >>kb) {
                if (key == "VmSize:") {
                    vmsize = std::stoi(value);
                    vmsize = vmsize/1000;
                    return std::to_string(vmsize);
                }

            }
        }
    }
    return std::string(); 
}

std::string LinuxParser::Uid(int pid) { 
    std::string key, value, uid, user, line;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "Uid:") {
                    uid = value;
                }

            }
        }
    }
    return uid; 
}

std::string LinuxParser::User(int pid) { 
    std::string usr, passwd, uid, line;
    std::ifstream stream (kPasswordPath);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            while (linestream >> usr >> passwd >> uid) {
                if (uid == LinuxParser::Uid(pid))
                    return usr;
            }
        }
    }
    return std::string();
}

long int LinuxParser::UpTime(int pid) { 
    std::string stat_str, line;
    long int start_time, uptime;
    std::vector<std::string> stat_list;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        while (linestream >> stat_str) {
            stat_list.push_back(stat_str);
        }
    }

    start_time = std::stol(stat_list[21]) / sysconf(_SC_CLK_TCK);
    uptime = LinuxParser::UpTime() - start_time;
    return uptime;
}