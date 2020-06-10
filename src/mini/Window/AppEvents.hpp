#pragma once
#include "mini/Box/Array.hpp"
#include "mini/Box/Bitset.hpp"

namespace mini::wnd
{
    enum class EventType : u8
    {
        Mouse_Left,
        Mouse_Right,

        Keyboard_Escape,
        Keyboard_W,
        Keyboard_E,

        PRESSABLE_END,

        Window_Close,
        Window_Resize,
        Window_Minimize,
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
            struct { s32 x, y; } pos;
            struct { u32 w, h; } size;
        };

        bool operator==(const EventType otherType) { return type == otherType; }
    };


    //!global arrays for wnd proc
    inline box::Array<Event, 10> events; //per frame max
    inline box::Bitset<EventType::PRESSABLE_END> pressed;
    inline u32 window_w = 0, window_h = 0;
    inline u32 mouse_x = 0, mouse_y = 0;

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

    //TODO: maybe some ConsumeEvent method

}//ns