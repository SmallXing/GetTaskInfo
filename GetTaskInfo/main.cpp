#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <thread>
#include <fstream>
#include "GetTaskInfo.h"
#include "log4z\logInit.h"

#include<string>
#include "threadsafe_queue.hpp"

#include <boost/algorithm/string.hpp>
#include "Task2Excel.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace boost::property_tree;

void GetXmlSet(std::set<std::string>& _set,uint32_t &_cycle)
{
    LOGI(__FUNCTION__);
    try
    {
        _set.clear();
        _cycle = 500;
        ptree pt;
        LOGI("��ʼ��ȡprocess.xml");
        read_xml("process.xml", pt);

        {
            LOGI("��ʼ��ȡmain.process");
            auto child = pt.get_child("main.process");
            for (auto pos : child)
            {
                LOGI("[process] " << pos.second.data());
                _set.insert(pos.second.data());
            }
        }

        {
            
            _cycle = pt.get<uint32_t>("main.cycle.TaskInfo", 500);
            LOGI("[cycle] " << _cycle);
        }



    }
    catch (...)
    {
        LOGE(__FUNCTION__ << "  ERROR");
    }

}

int main(int argc, char** argv)
{
    CLog4zInit::Initialize();
    LOGI("��ʼ����־��ɹ�");

    
    std::set<std::string> selectItem;
    uint32_t cycle;
    //= { "CleanDisk.exe *32", "CoreServer.exe *32", "System Idle Process"
    //"ServicesManager.exe *32","HintAMProxy.exe *32","SGUServer.exe *32",
    //"SpeedServer.exe *32","SGUClient.exe *32","DownLoadSVC.exe *32"};

    GetXmlSet(selectItem, cycle);
    
    CGetTaskInfo info(selectItem);

    LOGI("��ʼ��ʼ��CGetTaskInfo");
    if (!info.initialize())
    {
        LOGE("��ʼ��CGetTaskInfoʧ�ܣ������˳�");
        return 0;
    }

    LOGI("��ʼ��CGetTaskInfo�ɹ�");

    CTask2Excel task2Excel;
    LOGI("���ûص�����");
    info.setCallFun(std::bind(&CTask2Excel::fun, &task2Excel, std::placeholders::_1, std::placeholders::_2));
    task2Excel.start();

    LOGI("���Ѷ����ѿ�ʼ��������");

    while (1)
    {
        info.getTaskInfo();
        std::this_thread::sleep_for(std::chrono::milliseconds(cycle));
    }

    info.release();

    LOGI("���������˳�");

    return 0;
}