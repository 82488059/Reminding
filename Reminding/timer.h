#pragma once

#include <map>
#include <memory>
#include <list>
#include <condition_variable>
#include <mutex>


class CTimer
{
public:
    enum { em_unknow, em_hour, em_day, em_week, em_month };
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
    int Type() const { return m_type; }
    const SYSTEMTIME& Time()const { return m_time; }
    bool operator == (const SYSTEMTIME& rTime) {
        return m_time.wYear == rTime.wYear
            && m_time.wMonth == rTime.wMonth
            && m_time.wDay == rTime.wDay
            && m_time.wHour == rTime.wHour
            && m_time.wMinute == rTime.wMinute
            && m_time.wSecond == rTime.wSecond
            && m_time.wDayOfWeek == rTime.wDayOfWeek;
    }
    void Start(std::function<void()> task)
    {
        //std::thread([this, task]() {
        //    long dt = this->WillRing() - CTime::GetCurrentTime().GetTime();
        //    if (dt > 0)
        //        std::this_thread::sleep_for(std::chrono::seconds(dt));
        //    task();
        //    }
        //).detach();
    }
    const CString& Remark() const { return m_szRemark; }
private:
    int m_type{ em_unknow };
    int m_flags{ 0 };
    bool m_update{ true };
    SYSTEMTIME m_time{ 0 };
    __time64_t m_nextTime{ 0 };
    CString m_szRemark;
private:
};

typedef std::shared_ptr<CTimer> spTimer;

int GetTimeZone();
int GetThisMonthDays();
int GetMonthDays(int year, int month);
