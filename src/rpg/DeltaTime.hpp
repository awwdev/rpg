#pragma once
#include <chrono>

namespace rpg::dt
{
    using Clock = std::chrono::high_resolution_clock;

    inline std::size_t ticks = 0; //right word?
    inline double seconds = 0;
    inline Clock::time_point previous;

    inline double counter = 0; //to signal fps
    inline std::size_t fps = 0;


    inline void StartClock()
    {
        previous = Clock::now();
    }

    inline std::size_t CalcDeltaTimeFPS()
    {
        const auto now = Clock::now();
        const auto dur = now - previous;

        ticks    = dur.count();
        seconds  = std::chrono::duration<double>(dur).count();
        previous = now;

        counter += seconds;
        ++fps;

        if (counter >= 1)
        {
            counter -= 1;
            auto tmpFps = fps;
            fps = 0;
            return tmpFps;
        }

        return 0;
    }

}//ns