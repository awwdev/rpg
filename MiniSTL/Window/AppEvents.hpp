#pragma once
#include "MiniSTL/Box/Variant.hpp"
#include "MiniSTL/Box/Array.hpp"
#include "MiniSTL/Box/Bitset.hpp"

namespace mini::app
{
    enum class EventType : u8
    {
        Mouse_Left,
        Mouse_Right,

        Keyboard_Escape,
        Keyboard_W,

        PRESSABLE_END,

        Window_Close,
    };

    enum class EventState : u8
    {
        None, Released, Pressed
    };


    struct Event 
    {
        EventType  type;
        EventState state;
        union {
            struct { int x, y; } pos;
        };
    };


    ///global arrays for wnd proc
    inline box::Array <Event, 10> events; //per frame max
    inline box::Bitset<EventType::PRESSABLE_END> pressed;


    inline Event* CheckEvent(const EventType type, const EventState state = EventState::None)
    {
        FOR_ARRAY(events, i)
        {
            if (events[i].type == type && events[i].state == state)
                return &events[i];
        }
        return nullptr;
    }


    inline bool IsPressed(const EventType type)
    {
        return pressed.Test(type);
    }


}//ns