#include "stdafx.h"
#include "timerManager.h"
#include <time.h>


int CTimerManager::AnalysisTimer(bool flag)
{
    m_list.clear();
    for (auto& it : m_map)
    {
        if (flag)
        {
            it.second->Update();
        }
        it.second->WillRing();
        m_list.push_back(it.second);
    }
    Sort();
    return 0;
}

int CTimerManager::Sort()
{
    m_list.sort([](const spTimer a, const spTimer b)->bool { 
        return a->WillRing() < b->WillRing();
        });
    return 0;
}

spTimer CTimerManager::NextTimer()
{
    if (m_list.empty())
    {
        return nullptr;
    }
    return m_list.front();
}

bool CTimerManager::exist(int type, const SYSTEMTIME& stTime)
{
    for (const auto & it : m_map)
    {
        if (it.second->Type() == type)
        {
            if (*it.second==stTime)
            {
                return true;
            }
        }
    }
    return false;
}
