#pragma once

#include< windows.h >
#include< commctrl.h >
#include <iostream>
#include <string>
#include <cassert>

#include <functional>
#include <vector>
#include <string>
#include <memory>
#include <cstdint>
#include <set>

class CGetTaskInfo
{
public:
    CGetTaskInfo(const std::set<std::string>& selectItem);
    ~CGetTaskInfo();

public:
    bool initialize();
    void release();
    void setCallFun(std::function<void(std::vector<std::vector<std::string>>&, const std::string&)> fun);

    bool getTaskInfo();

protected:
    bool setTabViewIndex(std::uint32_t index)
    {
        // 自动切换到第二个tab
        HWND hTabView = (HWND)::FindWindowEx(_hwndLevel1, 0, "SysTabControl32", NULL);

        if (!hTabView) return false;
        std::size_t tabCount = ::SendMessage(hTabView, TCM_GETITEMCOUNT, NULL, NULL);

        //int curTab = ::SendMessage(hTabView, TCM_GETCURFOCUS, 0, NULL);
        BOOL ret = ::SendMessage(hTabView, TCM_SETCURFOCUS, index, NULL);
        if (ret) return false;
        return true;
    }
    
private:

    std::function<void(std::vector<std::vector<std::string>>&, const std::string&)> _fun;

    std::vector<std::vector<std::string>> _vec;
    std::set<std::string> _selectItem;


protected:
    HANDLE    _hProcess;
    LVITEM* _pointer;
    HWND    _hwnd, _hListview;
    LRESULT _headerhwnd; //listview控件的列头句柄
    LRESULT _rows;
    LRESULT _cols;  //listview控件中的行列数
    DWORD _ProcessID = NULL;
    DWORD _ThreadID = NULL;
    HWND _hwndLevel1;
};

