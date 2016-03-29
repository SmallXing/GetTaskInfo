#pragma once
#include "threadsafe_queue.hpp"

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <boost/date_time.hpp>

class CTask2Excel
{
public:
    CTask2Excel();
    ~CTask2Excel();

    bool start();
    void stop();
    
    void fun(std::vector<std::vector<std::string>>& vec, const std::string &strTime)
    {
        _queue.push(std::make_pair(strTime, vec));
    }

protected:
    void run();
    bool writeTaskInfo(std::shared_ptr<std::pair<std::string, std::vector<std::vector<std::string>>>> info);

private:
    CThreadSafeQueue<std::pair<std::string, std::vector<std::vector<std::string>>>> _queue;
    bool _run;
    std::shared_ptr<std::thread> _thread;

    std::ofstream _ofs;

    boost::gregorian::date _today;
    uint32_t _hour;
};

