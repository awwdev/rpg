//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/RenderGraph/IRenderer.hpp"
#include "mini/Window/AppEvents.hpp"
#include "mini/ECS/ComponentArray.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Utils/DeltaTime.hpp"
#include "mini/Memory/Allocator.hpp"

#include <charconv>

namespace mini::app::ui
{
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

}//ns