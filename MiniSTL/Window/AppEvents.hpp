#pragma once
#include "MiniSTL/Box/Variant.hpp"
#include "MiniSTL/Box/Array.hpp"
#include "MiniSTL/Box/Bitset.hpp"

namespace mini::app
{
    struct Event 
    {
        enum Type : u8
        {
            Mouse_Left,
            Mouse_Right,

            Keyboard_Escape,
            Keyboard_W,

            PRESSABLE_END,

            Window_Close,
        } 
        type;

        enum State : u8 { None, Released, Pressed } state;

        union {
            struct { int x, y; } pos;
        };
    };


    ///global arrays for wnd proc
    inline box::Array <Event, 10> events; //per frame max
    inline box::Bitset<Event::Type::PRESSABLE_END> pressed;


    inline Event* CheckEvent(const Event::Type type, const Event::State state = Event::State::None)
    {
        FOR_ARRAY(events, i)
        {
            if (events[i].type == type && events[i].state == state)
                return &events[i];
        }
        return nullptr;
    }


    inline bool IsPressed(const Event::Type type)
    {
        return pressed.Test(type);
    }


}//ns