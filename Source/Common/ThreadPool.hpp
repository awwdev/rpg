//https://github.com/awwdev

#pragma once
#include "Common/Container/Array.hpp"

#include <thread>
#include <atomic>
#include <mutex>
#include <functional>

namespace rpg::com {
    
template<auto THREAD_COUNT_T>
struct ThreadPool
{
    static constexpr idx_t THREAD_COUNT = (idx_t) THREAD_COUNT_T;

    struct Thread
    {
        std::thread thread;
        std::function<void()> task;
        std::mutex taskMutex;
    };

    std::atomic<bool> isPoolRunning;
    Thread threads [THREAD_COUNT_T];

    void Init()
    {
        isPoolRunning = true;
        FOR_CARRAY(threads, i) {
            auto& t = threads[i];
            t.thread = std::thread { [this, &t] {
                while (isPoolRunning)
                {
                    t.taskMutex.lock();
                    if (t.task){
                        t.task();
                        t.task = nullptr;
                    }
                    t.taskMutex.unlock();
                    //std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            }};
        }  
    }

    ~ThreadPool()
    {
        isPoolRunning = false;
        WaitForAllTasks();
        FOR_CARRAY(threads, i){
            if (threads[i].thread.joinable())
                threads[i].thread.join();
        } 
    }

    void AssignTask(const idx_t threadIdx, const std::function<void()>& task)
    {
        dbg::Assert(isPoolRunning, "pool is not running");
        auto& thread = threads[threadIdx];
        thread.taskMutex.lock();
        dbg::Assert(!thread.task, "cannot assign task, thread is already busy");
        thread.task = task;
        thread.taskMutex.unlock();
    }

    void WaitForAllTasks()
    {
        idx_t threadIdx = 0;
        while(true)
        {
            const auto idx = threadIdx;
            
            threads[idx].taskMutex.lock();
            if (!threads[idx].task){
                ++threadIdx;
            }
            threads[idx].taskMutex.unlock();

            if (threadIdx >= THREAD_COUNT)
                break;
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

};

}//ns