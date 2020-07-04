//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/Window/AppEvents.hpp"
#include "mini/ECS/ComponentArray.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/Box/Map.hpp"
#include "mini/Box/String.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Utils/DeltaTime.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Utils/Algorithms.hpp"

#include <charconv>

//TODO: input field
//TODO: console

namespace mini::app::ui
{
    using UI_ID = u32;
    constexpr u32 FULL_OPAQUE = 95;
    constexpr u32 WHITE  = 0;
    constexpr u32 BLACK  = 5;
    constexpr u32 DARKER = 4;
    constexpr u32 DARK   = 3;
    constexpr u32 GRAY   = 2;

    struct Window
    {
        utils::Rect<float> rect;
        box::String<20>    title;
        bool isDragged  = false;
        bool isResizing = false;
        s32 dragX, dragY;

        static constexpr u32 BAR_H = 24;
    };

    struct InputField
    {
        box::String<30> input;
        bool isActive = false;
        //TODO: conversion function to grab data of input field
    };

    struct UI
    {   
        Window window1    {};
        InputField input1 {};
    };

    template<u32 STRLEN_0>
    bool DrawButton(rendergraph::RenderGraph& renderGraph, const char(&str)[STRLEN_0], const utils::Rect<float>& pRect, const Window& wnd)
    {
        return DrawButton(renderGraph, str, { wnd.rect.x + pRect.x, wnd.rect.y + wnd.BAR_H + pRect.y, pRect.w, pRect.h });
    }

    template<u32 STRLEN_0>
    bool DrawButton(rendergraph::RenderGraph& renderGraph, const char(&str)[STRLEN_0], const utils::Rect<float>& rect)
    {
        const bool isMouseInside   = utils::IsPointInsideRect(wnd::mouse_x, wnd::mouse_y, rect);
        const bool isMouseReleased = wnd::CheckEvent(wnd::EventType::Mouse_Left, wnd::EventState::Released) != nullptr; 
        const bool isMousePressed  = wnd::IsPressed(wnd::EventType::Mouse_Left);

        //? QUAD
        uint32_t btnColorIdx;
        if (isMouseInside && isMousePressed) btnColorIdx = WHITE;
        else btnColorIdx = isMouseInside ? GRAY : DARK; //TODO: push some const stuff to shader that came from host

        renderGraph.uboText.Append(
            rendergraph::UniformData_Text { 
                .offset         = { rect.x, rect.y }, 
                .size           = { rect.w, rect.h },
                .colorIndex     = btnColorIdx,
                .textureIndex   = 95 //this is full opaque
            }
        );

        //? TEXT
        constexpr auto STRLEN = STRLEN_0 - 1; //don't consider \0 for rendering
        constexpr auto LETTER_SIZE  = 16;
        constexpr auto LETTER_SPACE = 8;
        constexpr auto TOTAL_STR_W = STRLEN_0 * LETTER_SPACE; //not sure why strlen0 works (+1)

        const auto str_x = rect.x + rect.w * 0.5f - TOTAL_STR_W * 0.5f;
        const auto str_y = rect.y + rect.h * 0.5f - LETTER_SIZE * 0.5f;

        for(auto i = 0; i < STRLEN; ++i) {
            renderGraph.uboText.Append(
                rendergraph::UniformData_Text { 
                    .offset         = { str_x + LETTER_SPACE * i, str_y }, 
                    .size           = { LETTER_SIZE, LETTER_SIZE },
                    .colorIndex     = (uint32_t)0,
                    .textureIndex   = (uint32_t)str[i] - 32 //ascii "text offset"
                }
            );
        }

        return isMouseInside && isMouseReleased;
    }

    inline void DrawFPS(rendergraph::RenderGraph& renderGraph, const utils::Rect<float>& rect = { 0, 0, 64, 24 })
    {
        //? quad
        renderGraph.uboText.Append(
            rendergraph::UniformData_Text { 
                .offset         = { rect.x, rect.y }, 
                .size           = { rect.w, rect.h },
                .colorIndex     = 4, 
                .textureIndex   = 95 //this is full opaque
            }
        );

        //? text
        const auto& fps = dt::fps;
        char fpsStr [10] { '\0 '};
        const auto res = std::to_chars(fpsStr, fpsStr + 10, fps);

        const auto STRLEN = strlen(fpsStr);
        const auto LETTER_SIZE  = 16;
        const auto LETTER_SPACE = 8;
        const auto TOTAL_STR_W = (STRLEN+1) * LETTER_SPACE;

        const auto str_x = rect.x + rect.w * 0.5f - TOTAL_STR_W * 0.5f;
        const auto str_y = rect.y + rect.h * 0.5f - LETTER_SIZE * 0.5f;
       
        for(auto i = 0; i < STRLEN; ++i) {
            renderGraph.uboText.Append(
                rendergraph::UniformData_Text { 
                    .offset         = { str_x + LETTER_SPACE * i, str_y }, 
                    .size           = { LETTER_SIZE, LETTER_SIZE },
                    .colorIndex     = (uint32_t)0,
                    .textureIndex   = (uint32_t)fpsStr[i] - 32 //ascii "text offset"
                }
            );
        }
    }

    struct WindowContext
    {
        utils::Rect<float> rect;
    };

    inline void DrawWindow(
        rendergraph::RenderGraph& renderGraph, 
        Window& wnd)
    {
        const auto& rect = wnd.rect;
        const utils::Rect<float> bar = { rect.x, rect.y, rect.w, wnd.BAR_H };
        const utils::Rect<float> resizer = { rect.x + rect.w - 8, rect.y + rect.h - 8, 8, 8 };

        const bool isMouseOnBar    = utils::IsPointInsideRect(wnd::mouse_x, wnd::mouse_y, bar);
        const bool isMouseOnResizer= utils::IsPointInsideRect(wnd::mouse_x, wnd::mouse_y, resizer);

        const bool isMouseHeld     = wnd::IsPressed(wnd::EventType::Mouse_Left);
        const bool isMousePressed  = wnd::CheckEvent(wnd::EventType::Mouse_Left, wnd::EventState::Pressed);
        const bool isMouseReleased = wnd::CheckEvent(wnd::EventType::Mouse_Left, wnd::EventState::Released);

        //? DRAGGING
        if (isMouseReleased)
        {
            wnd.isDragged  = false;
            wnd.isResizing = false;
        }
        if (isMousePressed)
        {
            wnd.dragX = wnd::mouse_x;
            wnd.dragY = wnd::mouse_y;
        }
        if (isMousePressed && isMouseOnBar) {
            wnd.isDragged = true;    
        }
        if (isMousePressed && isMouseOnResizer) {
            wnd.isResizing = true;    
        }

        if (wnd.isDragged || wnd.isResizing)
        {
            const s32 deltaX = wnd::mouse_x - wnd.dragX;
            const s32 deltaY = wnd::mouse_y - wnd.dragY;
            wnd.dragX = wnd::mouse_x;
            wnd.dragY = wnd::mouse_y;

            if (wnd.isDragged) //drag
            {
                wnd.rect.x += deltaX;
                wnd.rect.y += deltaY;
            }
            if (wnd.isResizing) //resize
            {
                wnd.rect.w += deltaX;
                wnd.rect.h += deltaY;
                utils::IfClamp<140, 600>(wnd.rect.w);
                utils::IfClamp<64 , 600>(wnd.rect.h);
            }
        }

        //? QUAD
        renderGraph.uboText.Append(
            rendergraph::UniformData_Text { 
                .offset         = { rect.x, rect.y }, 
                .size           = { rect.w, rect.h },
                .colorIndex     = DARKER,
                .textureIndex   = FULL_OPAQUE,
            }
        );
        //? TITLE BAR
        renderGraph.uboText.Append(
            rendergraph::UniformData_Text { 
                .offset         = { bar.x, bar.y }, 
                .size           = { bar.w, bar.h },
                .colorIndex     = BLACK,
                .textureIndex   = FULL_OPAQUE,
            }
        );
        //? RESIZER
        renderGraph.uboText.Append(
            rendergraph::UniformData_Text { 
                .offset         = { resizer.x, resizer.y }, 
                .size           = { resizer.w, resizer.h },
                .colorIndex     = isMouseOnResizer ? GRAY : DARK,
                .textureIndex   = FULL_OPAQUE,
            }
        );

        //? TITLE TEXT
        const auto STRLEN = wnd.title.Length(); //don't consider \0 for rendering
        const auto LETTER_SIZE  = 16;
        const auto LETTER_SPACE = 8;
        const auto TOTAL_STR_W = (wnd.title.Length() + 1) * LETTER_SPACE; //not sure why strlen0 works (+1)

        const auto str_x = rect.x + rect.w * 0.5f - TOTAL_STR_W * 0.5f;
        const auto str_y = rect.y + 24 * 0.5f - LETTER_SIZE * 0.5f;

        for(u32 i = 0; i < STRLEN; ++i) {
            renderGraph.uboText.Append(
                rendergraph::UniformData_Text { 
                    .offset         = { str_x + LETTER_SPACE * i, str_y }, 
                    .size           = { LETTER_SIZE, LETTER_SIZE },
                    .colorIndex     = WHITE,
                    .textureIndex   = (uint32_t)wnd.title[i] - 32 //ascii "text offset"
                }
            );
        }
    }


    

    template<u32 STRLEN_0>
    bool DrawInputField(rendergraph::RenderGraph& renderGraph, InputField& inputField, const char(&str)[STRLEN_0], const utils::Rect<float>& pRect, const Window& wnd)
    {
        return DrawInputField(renderGraph, inputField, str, { wnd.rect.x + pRect.x, wnd.rect.y + wnd.BAR_H + pRect.y, pRect.w, pRect.h });
    }

    template<u32 STRLEN_0>
    bool DrawInputField(
        rendergraph::RenderGraph& renderGraph,
        InputField& inputField,
        const char(&str)[STRLEN_0],
        const utils::Rect<float>& rect = { 0, 0, 64, 24 })
    {
        const utils::Rect<float> inputRect { rect.x + 64, rect.y, rect.w - 64, rect.h };
        const bool isMouseOnInput = utils::IsPointInsideRect(wnd::mouse_x, wnd::mouse_y, inputRect);
        const bool isMousePressed  = wnd::CheckEvent(wnd::EventType::Mouse_Left, wnd::EventState::Pressed);
       

        if (isMouseOnInput && isMousePressed) {
            inputField.isActive = true;
        }
        if (!isMouseOnInput && isMousePressed) {
            inputField.isActive = false; //probably does not cover all cases
        }

        if (inputField.isActive) {
            if (const auto* ev = wnd::CheckEvent(wnd::EventType::Keyboard_ASCII, wnd::EventState::Pressed)){
                if (ev->ascii == '\b')
                    inputField.input.Pop();
                else
                    inputField.input.Append(ev->ascii);
            }
        }

        //const bool isMouseOnBar    = utils::IsPointInsideRect(wnd::mouse_x, wnd::mouse_y, bar);
        //const bool isMouseOnResizer= utils::IsPointInsideRect(wnd::mouse_x, wnd::mouse_y, resizer);
        //const bool isMouseHeld     = wnd::IsPressed(wnd::EventType::Mouse_Left);
        //
        //

        //? INPUT FIELD
        renderGraph.uboText.Append(
            rendergraph::UniformData_Text { 
                .offset         = { inputRect.x, inputRect.y }, 
                .size           = { inputRect.w, inputRect.h },
                .colorIndex     = isMouseOnInput ? GRAY : DARK,
                .textureIndex   = FULL_OPAQUE,
            }
        );

        //? LABEL
        {
            const auto STRLEN = STRLEN_0 - 1; //don't consider \0 for rendering
            const auto LETTER_SIZE  = 16;
            const auto LETTER_SPACE = 8;
            const auto TOTAL_STR_W  = (STRLEN_0 + 1) * LETTER_SPACE; //not sure why strlen0 works (+1)

            const auto str_x = rect.x + 0;
            const auto str_y = rect.y + 24 * 0.5f - LETTER_SIZE * 0.5f;

            for(u32 i = 0; i < STRLEN; ++i) {
                renderGraph.uboText.Append(
                    rendergraph::UniformData_Text { 
                        .offset         = { str_x + LETTER_SPACE * i, str_y }, 
                        .size           = { LETTER_SIZE, LETTER_SIZE },
                        .colorIndex     = WHITE,
                        .textureIndex   = (uint32_t)str[i] - 32 //ascii "text offset"
                    }
                );
            }
        }

        //? INPUT
        {
            const auto STRLEN = inputField.input.Count() - 1; //don't consider \0 for rendering
            const auto LETTER_SIZE  = 16;
            const auto LETTER_SPACE = 8;
            const auto TOTAL_STR_W  = (inputField.input.Count() + 1) * LETTER_SPACE; //not sure why strlen0 works (+1)

            const auto str_x = rect.x + 64 + 8;
            const auto str_y = rect.y + 24 * 0.5f - LETTER_SIZE * 0.5f;

            for(u32 i = 0; i < STRLEN; ++i) {
                renderGraph.uboText.Append(
                    rendergraph::UniformData_Text { 
                        .offset         = { str_x + LETTER_SPACE * i, str_y }, 
                        .size           = { LETTER_SIZE, LETTER_SIZE },
                        .colorIndex     = WHITE,
                        .textureIndex   = (uint32_t)inputField.input[i] - 32 //ascii "text offset"
                    }
                );
            }
        }
        

        return false;
    }


}//ns