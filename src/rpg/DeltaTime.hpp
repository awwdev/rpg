#pragma once
#include <chrono>

namespace rpg::dt
{
    using Clock = std::chrono::high_resolution_clock;

    inline size_t ticks   = 0; //right word?
    inline double seconds = 0;
    inline Clock::time_point previous;


    inline void StartClock()
    {
        previous = Clock::now();
    }

    inline void CalcDeltaTime()
    {
        const auto now = Clock::now();
        const auto dur = now - previous;

        ticks   = dur.count();
        seconds = std::chrono::duration<double>(dur).count();

        previous = now;
    }

}//ns