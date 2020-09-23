//https://github.com/awwdev

#pragma once
#include "com/Container/Array.hpp"

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
        std::atomic<bool> active;
    };

    std::atomic<bool> isPoolRunning;
    Thread threads [THREAD_COUNT_T];

    void Start()
    {
        dbg::Assert(isPoolRunning == false, "started a non-stopped thread pool");
        isPoolRunning = true;
        FOR_CARRAY(threads, i) {
            auto& t = threads[i];
            t.thread = std::thread { [this, &t] {
                while (isPoolRunning)
                {
                    if (t.active){
                        t.task();
                        t.active = false;
                    }
                }
            }};
        }  
    }

    void Stop()
    {
        WaitForAllTasks();
        isPoolRunning = false;
    }

    ~ThreadPool()
    {
        Stop();
        FOR_CARRAY(threads, i){
            if (threads[i].thread.joinable())
                threads[i].thread.join();
        } 
    }

    void AssignTask(const idx_t threadIdx, const std::function<void()>& task)
    {
        dbg::Assert(isPoolRunning, "pool is not running");
        auto& thread = threads[threadIdx];
        dbg::Assert(thread.active == false, "cannot assign task, thread is already busy");
        thread.task = task;
        thread.active = true;
    }

    void WaitForAllTasks()
    {
        idx_t threadIdx = 0;
        while(true)
        {
            const auto idx = threadIdx;
            
            if (threads[idx].active == false){
                ++threadIdx;
            }

            if (threadIdx >= THREAD_COUNT)
                break;
        }
    }

};

}//ns