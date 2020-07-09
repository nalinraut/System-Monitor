#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
    /*
    Returns the aggregate CPU utilization
    */
    float Utilization();  

private:
    float _total_jiffies{0.0},
        _idle_jiffies{0.0},
        _util_jiffies{0.0},
        _usage{0.0};

    static float _total_jiffies_prev,
                _util_jiffies_prev;
};

#endif