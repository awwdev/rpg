#pragma once
#include "mini/Box/Array.hpp"
#include "mini/Box/Bitset.hpp"

namespace mini::wnd
{
    enum class EventType : u8
    {
        Mouse_Left,
        Mouse_Right,
        Mouse_Scroll,

        Keyboard_Escape,
        Keyboard_F1,
        Keyboard_ASCII,

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
        //TODO: better write dedicated ctors
        union {
            struct { s32 x, y; } pos; 
            struct { u32 w, h; } size;
            char   ascii;
            short  scrollDelta;
        };

        bool operator==(const EventType otherType) { return type == otherType; }
    };


    //!global arrays
    inline box::Array<Event, 10> events; //per frame max
    inline box::Bitset<EventType::PRESSABLE_END> pressed;
    inline box::Bitset<255> asciiPressed;
    //inline bool asciiPressed [255];
    inline u32 window_w, window_h;
    inline u32 mouse_client_x, mouse_client_y;
    inline s32 mouse_dx, mouse_dy;
    inline bool ui_mode = true;


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