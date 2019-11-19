#pragma once
#include <windows.h>
#include <map>
#include <memory>


struct timer
{
    enum {em_unknow, em_hour, em_day, em_week, em_month};
    int type{ em_unknow };
    CTime time;
    CString szRemark;
    int flags{0};
};

typedef std::shared_ptr<timer> spTimer;


class CTimerManager {

public:

    void RemoveTimer(int index)
    {
        m_map.erase(index);
    }
    int AddTimer(spTimer sp)
    {
        m_index++;

        m_map[m_index] = sp;

        return m_index;
    }

    
private:
    int m_index{ 0 };
    std::map<int, spTimer> m_map;
};