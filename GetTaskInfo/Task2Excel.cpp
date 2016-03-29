#include "Task2Excel.h"
#include "boost/date_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include <windows.h>
#include "./log4z/logInit.h"

#pragma comment(lib,"libboost_date_time-vc120-sgd-1_57.lib")
CTask2Excel::CTask2Excel()
{
 
}


CTask2Excel::~CTask2Excel()
{
}

uint32_t getCurHour()
{
    auto t = ::time(NULL);

    //auto gm = gmtime(&t);
    auto gm = localtime(&t);
    return gm->tm_hour;
}

bool CTask2Excel::start()
{
    LOGI(__FUNCTION__);
    _today = boost::gregorian::day_clock::local_day();
    
    std::string strTime = boost::gregorian::to_sql_string(_today);
    _hour = getCurHour();
    CreateDirectory("./out/",NULL);
    _ofs.open(("./out/" + strTime + " " + boost::lexical_cast<std::string>(_hour)+".csv").c_str(), std::ios_base::app);

    _run = true;
    _thread.reset(new std::thread(&CTask2Excel::run, this), [this](std::thread* t){ _run = false; t->join(); delete t; });

    return _thread ? true : false;
}

void CTask2Excel::stop()
{
    LOGI(__FUNCTION__);
    _queue.stop();
    _thread.reset();
    _ofs.close();
}



void CTask2Excel::run()
{
    LOGI(__FUNCTION__);
    while (_run)
    {
        auto pos = _queue.wait_and_pop();
        
        if (!pos)
        {
            LOGW("此信息信息有问题，可能是程序要退出");
            continue;
        }

        if (writeTaskInfo(pos))
        {
           
        }
    }
}

#include <ctime>

bool CTask2Excel::writeTaskInfo(std::shared_ptr<std::pair<std::string, std::vector<std::vector<std::string>>>> info)
{
    LOGD(__FUNCTION__);

    if (boost::gregorian::day_clock::local_day() != _today || getCurHour() != _hour)
    {
        LOGI("新的一天开始，重新生成csv文件");
        std::string strTime = boost::gregorian::to_sql_string(_today = boost::gregorian::day_clock::local_day());
        _hour = getCurHour();
        //std::string strTime = boost::posix_time::to_iso_string(_today = boost::posix_time::second_clock::local_time());
        _ofs.close();
        _ofs.open(("./out/" + strTime + " " + boost::lexical_cast<std::string>(_hour)+".csv").c_str(), std::ios_base::app);
        if (!_ofs.is_open())
        {
            LOGE("文件打开失败:" << strTime);
        }
    }

    
    for (auto pos1 : info->second)
    {
        std::string str = info->first;
        for (auto pos2 : pos1)
        {
            str += ",";
            boost::erase_all(pos2, ",");
            str += pos2;
        }
        str += "\n";
        _ofs.write(str.c_str(), str.length());
    }

    return true;
}
