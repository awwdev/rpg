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

#include <charconv>

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
        bool isDragging = false;
        s32 dragX, dragY;
    };

    struct UI
    {   
        Window window1 {};
    };



    template<u32 STRLEN_0>
    bool DrawButton(rendergraph::RenderGraph& renderGraph, const char(&str)[STRLEN_0], const utils::Rect<float>& rect)
    {
        const bool isMouseInside   = utils::IsPointInsideRect(wnd::mouse_x, wnd::mouse_y, rect);
        const bool isMouseReleased = wnd::CheckEvent(wnd::EventType::Mouse_Left, wnd::EventState::Released) != nullptr; 
        const bool isMousePressed  = wnd::IsPressed(wnd::EventType::Mouse_Left);

        //? QUAD
        uint32_t btnColorIdx;
        if (isMouseInside && isMousePressed) btnColorIdx = 2;
        else btnColorIdx = isMouseInside ? 3 : 4; //TODO: push some const stuff to shader that came from host

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

    void DrawWindow(
        rendergraph::RenderGraph& renderGraph, 
        Window& wnd)
    {
        const auto& rect = wnd.rect;
        const utils::Rect<float> bar = { rect.x, rect.y, rect.w, 24 };

        const bool isMouseOnBar   = utils::IsPointInsideRect(wnd::mouse_x, wnd::mouse_y, bar);
        const bool isMouseHeld    = wnd::IsPressed(wnd::EventType::Mouse_Left);
        const bool isMousePressed = wnd::CheckEvent(wnd::EventType::Mouse_Left, wnd::EventState::Pressed);

        //? DRAGGING
        if (isMouseOnBar && isMousePressed)
        {
            wnd.dragX = wnd::mouse_x;
            wnd.dragY = wnd::mouse_y;
        }
        if (isMouseOnBar && isMouseHeld)
        {
            const s32 deltaX = wnd::mouse_x - wnd.dragX;
            const s32 deltaY = wnd::mouse_y - wnd.dragY;
            wnd.dragX = wnd::mouse_x;
            wnd.dragY = wnd::mouse_y;

            wnd.rect.x += deltaX;
            wnd.rect.y += deltaY;
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



}//ns