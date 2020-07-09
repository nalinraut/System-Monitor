#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long int seconds) { 
    long int HH{seconds/3600};
    long int H_re{seconds%3600};
    long int MM{H_re/60};
    long int SS{H_re%60};
    string HH_str{std::to_string(HH)};
    string MM_str{std::to_string(MM)};
    string SS_str{std::to_string(SS)};
    string time{HH_str+':'+MM_str+':'+SS_str};
    return time; 
}