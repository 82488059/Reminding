#pragma once
#include <windows.h>
#include <map>
#include <memory>
#include <list>

#include "Timer.h"


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
    spTimer NextTimer();
    bool exist(int type, const SYSTEMTIME & stTime);

private:
    int m_index{ 0 };
    std::map<int, spTimer> m_map;
    std::list<spTimer> m_list;
};