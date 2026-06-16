#pragma once
#include "Engine_Defines.h"

NS_BEGIN(Engine)
class CThreadPool
{
private:
    CThreadPool();
public:
    ~CThreadPool();

    HRESULT Initialize(size_t workerCount);
    void Enqueue(function<void()> job);
    void WaitAll();
    void Shutdown();

private:
    vector<thread> m_Workers;
    queue<function<void()>> m_Jobs;
    
    mutex m_Mutex;
    condition_variable m_CV;
    
    condition_variable m_WaitCV;

    _bool m_Stop = false;
    atomic<size_t> m_RunningJobs = 0;

public:
    static unique_ptr<CThreadPool> Create(size_t workerCount);

};
NS_END