//https://github.com/awwdev

#pragma once
#include <chrono>

namespace mini::dt {
    
using Clock = std::chrono::high_resolution_clock;

inline std::size_t ticks = 0; //right word?
inline double seconds = 0;
inline Clock::time_point previous = Clock::now();

inline double counter = 0; //to signal fps
inline std::size_t frames = 0;
inline std::size_t fps = 0;

inline void UpdateFPS()
{
    const auto now = Clock::now();
    const auto dur = now - previous;

    ticks    = dur.count();
    seconds  = std::chrono::duration<double>(dur).count();
    previous = now;

    counter += seconds;
    ++frames;

    if (counter >= 1)
    {
        counter -= 1;
        fps = frames;
        //LOG("fps", fps);
        frames = 0;
    }
}

}//ns