#include "GetTaskInfo.h"
#include <thread>
#include <set>
#include "log4z/logInit.h"

CGetTaskInfo::CGetTaskInfo(const std::set<std::string>& selectItem)
: _selectItem(selectItem)
{
    LOGI(__FUNCTION__);
}


CGetTaskInfo::~CGetTaskInfo()
{
    LOGI(__FUNCTION__);
    release();
}

bool CGetTaskInfo::initialize()
{
    LOGI(__FUNCTION__);
    uint8_t execIndex = 0;// �����Ĵ���
    do 
    {
        LOGD("��ʼ�������������");
        _hwndLevel1 = (HWND)::FindWindow("#32770", "Windows ���������");
        if (!_hwndLevel1)
        {// windows ���������û������  taskmgr.exe
            LOGD("û�в��ҵ�������������̣���������һ��");
            ShellExecute(NULL, NULL, "taskmgr.exe", NULL, NULL, SW_HIDE);

            LOGI("�� HIDE��ʽ�������������");
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        if (++execIndex >= 5)
        {
            LOGE("����������� 5�ζ�ʧ�ܣ���ʼ�����ɹ�");
            return false;
        }

    } while (!_hwndLevel1);

    LOGD("�л������������һ��tab");
    if (!setTabViewIndex(1))
    {
        LOGE("�л�tab ʧ�ܣ���ʼ�����ɹ�");
        return false;
    }
    _hwnd = (HWND)::FindWindowEx(_hwndLevel1, 0, "#32770", NULL);
    if (!_hwnd)
    {
        LOGE("���� #32770����ʧ�ܣ���ʼ�����ɹ�");
        return false;
    }

    //���̽��洰�ڵľ��,ͨ��SPY��ȡ
    _hListview = (HWND)::FindWindowEx(_hwnd, 0, "SysListView32", NULL);
    if (!_hListview)
    {
        LOGE("���� SysListView32����ʧ�ܣ���ʼ�����ɹ�");
        return false;
    }

    //listview����ͷ���
    _headerhwnd = ::SendMessage(_hListview, LVM_GETHEADER, NULL, NULL);

    if (!_headerhwnd)
    { 
        LOGE("���� list��ͷ��Ϣʧ�ܣ���ʼ�����ɹ�");
        return false;
    }
    //������:���̵�����
    _rows = ::SendMessage(_hListview, LVM_GETITEMCOUNT, NULL, NULL);
    
    //�б�����
    _cols = ::SendMessage((HWND)_headerhwnd, HDM_GETITEMCOUNT, NULL, NULL);
    if (!_cols)
    {
        LOGE("��ȡ��������Ϣʧ�ܣ���ʼ�����ɹ�");
        return false;
    }


    _ThreadID = GetWindowThreadProcessId(_hListview, &_ProcessID);

    //�򿪲��������
    _hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _ProcessID);
    if (!_hProcess)
    {
        LOGE("OpenProcess��Ϣʧ�ܣ���ʼ�����ɹ�");
        return false;
    }

    //���������ڴ���
    _pointer = (LVITEM*)VirtualAllocEx(_hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
    if (!_pointer)
    {
        LOGE("���������ڴ��� ʧ�ܣ���ʼ�����ɹ�");
        return false;
    }


    return true;
}

void CGetTaskInfo::release()
{
    LOGI(__FUNCTION__);
    CloseHandle(_hProcess);//�رմ򿪵Ľ��̶���
    ::SendMessage((HWND)_hwndLevel1, WM_CLOSE, 0, 0);
}

void CGetTaskInfo::setCallFun(std::function<void(std::vector<std::vector<std::string>>&, const std::string&)> fun)
{
    LOGI(__FUNCTION__);
    _fun = fun;
}


bool CGetTaskInfo::getTaskInfo()
{
    _vec.clear();
    if (!setTabViewIndex(1)) return false;
    for (int i = 0; i < _rows; i++)
    {
        std::vector<std::string> vec;
        for (int j = 0; j < _cols; j++)
        {
            LVITEM vItem;
            vItem.mask = LVIF_TEXT;    //˵��pszText����Ч��
            vItem.iItem = i;        //�к�
            vItem.iSubItem = j;        //�к�
            vItem.cchTextMax = 512;    //���ܴ洢�������ı�Ϊ256�ֽ�
            char* pItem = (char*)VirtualAllocEx(_hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
            if (!pItem) return false;
            std::shared_ptr<char> _auto(pItem, [this](char* item){VirtualFreeEx(_hProcess, item, 0, MEM_RELEASE); });

            vItem.pszText = pItem;

            BOOL ret = WriteProcessMemory(_hProcess, _pointer, &vItem, sizeof(LVITEM), NULL);
            ret = ::SendMessage(_hListview, LVM_GETITEM, (WPARAM)i, (LPARAM)_pointer);
            char ItemBuf[512];
            memset(ItemBuf, 0, 512);
            ret = ReadProcessMemory(_hProcess, pItem, ItemBuf, 512, NULL);
            
            if (j == 0)
            {
                if (_selectItem.empty())
                {// ѡ����Ϊ�գ���ʾ���н���ȫ����¼

                }else if (_selectItem.find(ItemBuf) == _selectItem.end())
                {
                    break;
                }
            }

            vec.push_back(std::string(ItemBuf));
        }
        if (!vec.empty())
            _vec.push_back(vec);
    }

    {
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        unsigned long long now = ft.dwHighDateTime;
        now <<= 32;
        now |= ft.dwLowDateTime;
        now /= 10;
        now -= 11644473600000000ULL;
        now /= 1000;
        auto _time = now / 1000;
        auto _precise = (unsigned int)(now % 1000);

        auto timeToTm = [](time_t t)
        {
            struct tm tt = { 0 };
            localtime_s(&tt, &t);
            return tt;
        };
        tm tt = timeToTm(_time);

        char _content[2048];
        int ret = _snprintf_s(_content, 4000, _TRUNCATE, "(%d-%02d-%02d %02d:%02d:%02d.%03u)",
            tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, _precise);
        if (_fun) _fun(_vec, _content);
    }

    

    return true;
}