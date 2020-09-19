//https://github.com/awwdev

#pragma once
#include "Common/Container/Array.hpp"

#include <thread>
#include <atomic>
#include <mutex>
#include <functional>

namespace rpg::com {
    
template<auto THREAD_COUNT_T, class TaskArgs>
struct ThreadPool
{
    static constexpr idx_t THREAD_COUNT = (idx_t) THREAD_COUNT_T;

    using TaskFn = void(TaskArgs&&);

    struct Thread
    {
        std::atomic<bool> taskFinished;
        std::jthread thread;
        std::function<void()> task;
        std::mutex taskMutex;
    };

    std::atomic<bool> isPoolRunning { true };
    Thread threads [THREAD_COUNT_T];

    ThreadPool() 
    {
        FOR_CARRAY(threads, i) {
            auto& t = threads[i];
            t.taskFinished = false;
            t.task = nullptr;
            t.thread = std::jthread { [this, &t] {
                while (isPoolRunning)
                {
                    t.taskMutex.lock();
                    if (t.task != nullptr){
                        t.task();
                        t.task = nullptr;
                    }
                    t.taskMutex.unlock();
                }
            }};
        }  
    }

    ~ThreadPool()
    {
        isPoolRunning = false;
    }

    void AssignTask(const idx_t threadIdx, const std::function<void()>& task)
    {
        auto& thread = threads[threadIdx];
        thread.taskMutex.lock();
        dbg::Assert(threads[threadIdx].task == nullptr, "cannot assign task, thread is already busy");
        thread.task = task;
        thread.taskMutex.unlock();
    }

    void WaitForAllFinished()
    {
        idx_t threadIdx = 0;
        while(true)
        {
            if (threads[threadIdx].task == nullptr){
                ++threadIdx;
            }
            if (threadIdx >= THREAD_COUNT)
                break;
        }
    }

};

}//ns