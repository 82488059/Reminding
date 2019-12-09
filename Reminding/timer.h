#pragma once
#include <windows.h>
#include <map>
#include <memory>
#include <list>


int GetTimeZone();
int GetThisMonthDays();
int GetMonthDays(int year, int month);


class timer
{
public:
    enum {em_unknow, em_hour, em_day, em_week, em_month};
    void Set(int type, SYSTEMTIME& time, const CString& remark, int flags = 0)
    {
        m_type = type;
        m_time = time;
        m_szRemark = remark;
        m_flags = flags;
        m_update = true;
    }
    void Update()
    {
        m_update = true;
    }

    __time64_t WillRing();


private:
    int m_type{ em_unknow };
    int m_flags{ 0 };
    bool m_update{ true };
    SYSTEMTIME m_time;
    __time64_t m_nextTime;
    CString m_szRemark;
private:

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

    int AnalysisTimer(bool flag = false);

    int Sort();

private:
    int m_index{ 0 };
    std::map<int, spTimer> m_map;
    std::list<spTimer> m_list;
};