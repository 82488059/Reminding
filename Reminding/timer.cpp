#include "stdafx.h"
#include "timer.h"
#include <time.h>


int CTimerManager::AnalysisTimer()
{


    return 0;
}

__time64_t timer::WillRing()
{
    CTime now = CTime::GetCurrentTime();

    static int timezone = GetTimeZone();

    SYSTEMTIME stLocal;
    GetLocalTime(&stLocal);

    __time64_t now64 = now.GetTime();
    __time64_t next64 = now.GetTime();

    switch (m_type)
    {
    case em_hour:
    {
        next64 = next64 - (next64 % 3600);
        WORD nsec = (now64 % 3600);
        WORD sec = m_time.wMinute * 60 + m_time.wSecond;
        if (nsec < sec)
        {
            next64 += sec;
        }
        else
        {
            next64 += sec + 3600;
        }
        m_nextTime = next64;
        CTime t(next64);
        TRACE(_T("%s\n"), t.Format(_T("%Y-%m-%d %H:%M:%S %w-%A")));
        break;
    }


    case em_day:
    {
        next64 = next64 - (next64 % 86400);
        DWORD zsec = timezone * 3600;
        DWORD nowsec = now64 % 86400;
        WORD nextsec = m_time.wHour * 60 * 60 + m_time.wMinute * 60 + m_time.wSecond - zsec;
        if (nowsec > nextsec)
        {
            next64 += 86400 + nextsec;
        }
        else
        {
            next64 += nextsec;
        }
        m_nextTime = next64;
        CTime t(next64);
        TRACE(_T("%s\n"), t.Format(_T("%Y-%m-%d %H:%M:%S %w-%A")));
        break;
    }

    case em_week:
    {
        next64 = next64 - (next64 % 86400);
        int week = now.GetDayOfWeek();
        int nday = (m_time.wDayOfWeek - week + 7) % 7;
        WORD nsec = m_time.wHour * 60 * 60 + m_time.wMinute * 60 + m_time.wSecond;
        nsec += nday * 86400;
        next64 += nsec;
        CTime t(next64);

        TRACE(_T("%s\n"), t.Format(_T("%Y-%m-%d %H:%M:%S %w-%A")));

        m_nextTime = next64;
        break;
    }

    case em_month:
    {
        int nDay = GetThisMonthDays();
        m_time.wDay > nDay;

        break;
    }

    default:
        m_nextTime = 0;
        break;
    }
    return m_nextTime;
}

int GetTimeZone()
{
    static int n = ~0;
    if (~0 == n)
    {
        SYSTEMTIME t1, t2;
        GetLocalTime(&t1);
        GetSystemTime(&t2);
        n = t1.wHour - t2.wHour;
    }
    return n;
}

int GetThisMonthDays()
{
    COleDateTime time, nextMonth;
    SYSTEMTIME stLocal;
    GetLocalTime(&stLocal);
    time.GetAsSystemTime(stLocal);
    time.SetDateTime(time.GetYear(), time.GetMonth(), 1, 0, 0, 0);
    if (time.GetMonth() >= 12)
    {
        nextMonth.SetDateTime(time.GetYear() + 1, 1, 1, 0, 0, 0);
    }
    else
    {
        nextMonth.SetDateTime(time.GetYear(), time.GetMonth() + 1, 1, 0, 0, 0);
    }
    COleDateTimeSpan ts = nextMonth - time;
    return ts.GetDays();
}

int GetMonthDays(int year, int month)
{
    if (month > 12 || month < 1)
    {
        return -1;
    }
    COleDateTime time, nextMonth;
    time.SetDateTime(year, month, 1, 0, 0, 0);

    if (time.GetMonth() >= 12)
    {
        nextMonth.SetDateTime(time.GetYear() + 1, 1, 1, 0, 0, 0);
    }
    else
    {
        nextMonth.SetDateTime(time.GetYear(), time.GetMonth() + 1, 1, 0, 0, 0);
    }
    COleDateTimeSpan ts = nextMonth - time;
    return ts.GetDays();
}
