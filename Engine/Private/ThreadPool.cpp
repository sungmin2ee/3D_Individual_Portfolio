#include "ThreadPool.h"

CThreadPool::CThreadPool()
{
}

CThreadPool::~CThreadPool()
{
    Shutdown();
}

HRESULT CThreadPool::Initialize(size_t workerCount)
{
    m_Stop = false;

    for (size_t i = 0; i < workerCount; ++i)
    {
        m_Workers.emplace_back([this]()
            {
                while (true)
                {
                    std::function<void()> job;

                    {
                        std::unique_lock<std::mutex> lock(m_Mutex);

                        m_CV.wait(lock, [this]()
                            {
                                return m_Stop || !m_Jobs.empty();
                            });

                        if (m_Stop && m_Jobs.empty())
                            return;

                        job = std::move(m_Jobs.front());
                        m_Jobs.pop();

                        ++m_RunningJobs;
                    }

                    job();

                    {
                        std::lock_guard<std::mutex> lock(m_Mutex);

                        --m_RunningJobs;

                        if (m_Jobs.empty() && m_RunningJobs == 0)
                        {
                            m_WaitCV.notify_all();
                        }
                    }
                }
            });
    }
    return S_OK;
}

void CThreadPool::Enqueue(std::function<void()> job)
{
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Jobs.push(std::move(job));
    }

    m_CV.notify_one();
}

void CThreadPool::WaitAll()
{
    std::unique_lock<std::mutex> lock(m_Mutex);

    m_WaitCV.wait(lock, [this]()
        {
            return m_Jobs.empty() && m_RunningJobs == 0;
        });
}

void CThreadPool::Shutdown()
{
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Stop = true;
    }

    m_CV.notify_all();

    for (auto& worker : m_Workers)
    {
        if (worker.joinable())
            worker.join();
    }

    m_Workers.clear();
}
unique_ptr<CThreadPool> CThreadPool::Create(size_t workerCount)
{
    auto		pInstance = unique_ptr<CThreadPool>(new CThreadPool());

    if (FAILED(pInstance->Initialize(workerCount)))
    {
        MSG_BOX("Failed to Created : CThreadPool");
        return nullptr;
    }

    return pInstance;
}

