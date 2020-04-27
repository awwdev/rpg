#pragma once
#include "MiniSTL/Box/Variant.hpp"
#include "MiniSTL/Box/Array.hpp"
#include "MiniSTL/Box/Bitset.hpp"

namespace mini::app
{
    ///EVENTS TYPES

    enum class EvWindow
    {
        Close,
        ENUM_COUNT
    };

    enum class EvMouse
    {
        Pressed_LEft,
        Pressed_Middle,
        Pressed_Right,
        PRESSED_COUNT,

        Released_Left,
        Released_Middle,
        Released_Right,
    };

    enum class EvKeyboard
    {
        Pressed_Escape,
        Pressed_W,
        PRESSED_COUNT,

        Released_Escape,
        Released_W,
    };

    ///EVENT DATA

    template<class EVENT_T>
    struct EventData
    {
        EVENT_T type;

        union { //todo variant, also test
            struct { int x, y; } pos;
            struct { int w, h; } size;
        };

        inline bool operator==(const EVENT_T pType) { return type == pType; }
    };

    ///GLOBAL ARRAYS

    //global is needed because of global wnd procedure 
    inline box::Array<EventData<EvWindow>, 10>    windowEvents;
    inline box::Array<EventData<EvKeyboard>, 10>  keyboardEvents;
    inline box::Array<EventData<EvMouse>, 10>     mouseEvents;

    //keeping track of pressed input 
    inline box::Bitset<EvKeyboard::PRESSED_COUNT> keyboardDown;
    inline box::Bitset<EvMouse::PRESSED_COUNT>    mouseDown;

    ///HELPER

    template<class T> inline auto CheckEvent(const T type)
    {
        if constexpr (std::is_same_v<T, EvWindow>)    return windowEvents.Contains(type);
        if constexpr (std::is_same_v<T, EvMouse>)     return mouseEvents.Contains(type);
        if constexpr (std::is_same_v<T, EvKeyboard>)  return keyboardEvents.Contains(type);
    }

    template<class T> inline bool IsPressed(const T type)
    {
        if constexpr (std::is_same_v<T, EvKeyboard>)  return keyboardDown.Test(type);
        if constexpr (std::is_same_v<T, EvMouse>)     return mouseDown.Test(type);
    }

    inline void ClearEvents()
    {
        windowEvents.Clear();
        keyboardEvents.Clear();
        mouseEvents.Clear();
    }

}//ns