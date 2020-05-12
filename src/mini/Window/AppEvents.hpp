#pragma once
#include "mini/Box/Array.hpp"
#include "mini/Box/Bitset.hpp"

namespace mini::app
{
    //TODO: think about consuming (removing) events, so they are not passed const
    //usage of layers ?

    enum class EventType : u8
    {
        Mouse_Left,
        Mouse_Right,

        Keyboard_Escape,
        Keyboard_W,
        Keyboard_E,

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
    inline box::Array<Event, 10> events; //per frame max
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