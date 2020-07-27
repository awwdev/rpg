//https://github.com/awwdev

#pragma once
#include "mini/Memory/Allocator.hpp"
#include "mini/Window/AppEvents.hpp"
#include "mini/Utils/Types.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/Utils/DeltaTime.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Utils/PrimitiveMeshes.hpp"
#include "mini/Box/Map.hpp"
#include "mini/Box/String.hpp"
#include "mini/Rendering/Camera.hpp"
#include "mini/ECS/ComponentArray.hpp"

#include <charconv>
#undef DrawText

namespace mini::app::ui
{
    using namespace rendering;

    //? CONST (consistency)
    constexpr float LETTER_SIZE  = 16;
    constexpr float LETTER_SPACE = 8;

    //TODO: if branch in shader (if usesTexture) or just opaque and always sampling
    //TODO: use all textures of ascii even if never used (no ascii offset necessary then)
    constexpr u32 FULL_OPAQUE  = 95;
    constexpr u32 ASCII_OFFSET = 32; //texture begins at 0 with usable letters

    enum Colors : u32
    {
        WHITE,
        GREEN,
        RED,
        BLACK1,
        BLACK2,
        BLACK3,
        BLACK4,
        BLACK5,
    };
    
    inline void DrawTextCentered(
        RenderGraph& renderGraph,
        const utils::Rect<float>& rect, 
        chars_t str, const u32 len, 
        const Colors col = WHITE)
    {
        const auto STRLEN = len;
        const auto TOTAL_STR_W = (STRLEN + 1) * LETTER_SPACE;
        const auto str_x  = rect.x + rect.w * 0.5f - TOTAL_STR_W * 0.5f;
        const auto str_y  = rect.y + rect.h * 0.5f - LETTER_SIZE * 0.5f;

        for(u32 i = 0; i < STRLEN; ++i) {
            renderGraph.ui_ubo.AppendData(
                UI_UniformData { 
                    .rect         = { str_x + LETTER_SPACE * i, str_y, LETTER_SIZE, LETTER_SIZE },
                    .colorIndex   = col,
                    .textureIndex = str[i] - ASCII_OFFSET
                }
            );
        }
    }

    inline void DrawText(
        RenderGraph& renderGraph,
        const float x, const float y,
        chars_t str, const u32 len, 
        const Colors col = WHITE)
    {
        for(u32 i = 0; i < len; ++i) {
            renderGraph.ui_ubo.AppendData(
                UI_UniformData { 
                    .rect         = { x + LETTER_SPACE * i, y, LETTER_SIZE, LETTER_SIZE },
                    .colorIndex   = col,
                    .textureIndex = str[i] != '\0' ? str[i] - ASCII_OFFSET : ' ' - ASCII_OFFSET
                }
            );
        }
    }

    inline void DrawFPS(RenderGraph& renderGraph, const utils::Rect<float>& rect = { 0, 0, 48, 20 })
    {
        renderGraph.ui_ubo.AppendData(
            rendering::UI_UniformData { 
                .rect           = rect,
                .colorIndex     = BLACK1, 
                .textureIndex   = FULL_OPAQUE
            }
        );

        char fpsStr [10] { '\0 '};
        const auto res = std::to_chars(fpsStr, fpsStr + 10, dt::fps);
        DrawTextCentered(renderGraph, rect, fpsStr, (u32)strlen(fpsStr), GREEN);
    }

    struct Window
    {
        utils::Rect<float> rect;
        utils::Rect<float> limits;
        box::String<20>    title;
        bool isDragged;
        bool isResizing;
        s32 dragX, dragY;
        
        static constexpr u32 BAR_H = 20;
    };

    inline void DrawWindow(RenderGraph& renderGraph, Window& wnd)
    {
        const utils::Rect<float> bar     = { wnd.rect.x, wnd.rect.y, wnd.rect.w, wnd.BAR_H };
        const utils::Rect<float> resizer = { wnd.rect.x + wnd.rect.w - 8, wnd.rect.y + wnd.rect.h - 8, 8, 8 };

        const bool isMouseOnBar    = utils::IsPointInsideRect(wnd::mouse_client_x, wnd::mouse_client_y, bar);
        const bool isMouseOnResizer= utils::IsPointInsideRect(wnd::mouse_client_x, wnd::mouse_client_y, resizer);

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
            wnd.dragX = wnd::mouse_client_x;
            wnd.dragY = wnd::mouse_client_y;
        }
        if (isMousePressed && isMouseOnBar) {
            wnd.isDragged = true;    
        }
        if (isMousePressed && isMouseOnResizer) {
            wnd.isResizing = true;    
        }

        if (wnd.isDragged || wnd.isResizing)
        {
            const s32 deltaX = wnd::mouse_client_x - wnd.dragX;
            const s32 deltaY = wnd::mouse_client_y - wnd.dragY;
            wnd.dragX = wnd::mouse_client_x;
            wnd.dragY = wnd::mouse_client_y;

            if (wnd.isDragged)
            {
                wnd.rect.x += deltaX;
                wnd.rect.y += deltaY;
            }
            if (wnd.isResizing)
            {
                wnd.rect.w += deltaX;
                wnd.rect.h += deltaY;
                utils::Clamp(wnd.rect.w, wnd.limits.x, wnd.limits.w);
                utils::Clamp(wnd.rect.h, wnd.limits.y, wnd.limits.h);
            }
        }

        //? WINDOW
        renderGraph.ui_ubo.AppendData(
            rendering::UI_UniformData { 
                .rect           = wnd.rect,
                .colorIndex     = BLACK2,
                .textureIndex   = FULL_OPAQUE,
            }
        );
        //? TITLE BAR
        renderGraph.ui_ubo.AppendData(
            rendering::UI_UniformData { 
                .rect           = bar,
                .colorIndex     = BLACK1,
                .textureIndex   = FULL_OPAQUE,
            }
        );
        //? RESIZER
        renderGraph.ui_ubo.AppendData(
            rendering::UI_UniformData { 
                .rect           = resizer, 
                .colorIndex     = isMouseOnResizer ? RED : BLACK3,
                .textureIndex   = FULL_OPAQUE,
            }
        );

        //? TITLE TEXT
        DrawTextCentered(renderGraph,bar, wnd.title.dataPtr, wnd.title.Length());
    }


 

    struct InputField
    {
        box::String<100> str;
        bool isActive = false;
        s32 GetInt() const { return std::atoi(str.dataPtr); }
    };



    template<u32 STRLEN_0>
    bool DrawButton(RenderGraph& renderGraph, const char(&str)[STRLEN_0], const utils::Rect<float>& pRect, const Window& wnd)
    {
        return DrawButton(renderGraph, str, { wnd.rect.x + pRect.x, wnd.rect.y + wnd.BAR_H + pRect.y, pRect.w, pRect.h });
    }

    template<u32 STRLEN_0>
    bool DrawButton(RenderGraph& renderGraph, const char(&str)[STRLEN_0], const utils::Rect<float>& rect)
    {
        const bool isMouseInside   = utils::IsPointInsideRect(wnd::mouse_client_x, wnd::mouse_client_y, rect);
        const bool isMouseReleased = wnd::CheckEvent(wnd::EventType::Mouse_Left, wnd::EventState::Released); 
        const bool isMousePressed  = wnd::IsPressed(wnd::EventType::Mouse_Left);

        //? QUAD
        uint32_t btnColorIdx;
        if (isMouseInside && isMousePressed) btnColorIdx = BLACK5;
        else btnColorIdx = isMouseInside ? BLACK4 : BLACK3;

        renderGraph.ui_ubo.AppendData(
            rendering::UI_UniformData { 
                .rect           = rect, 
                .colorIndex     = btnColorIdx,
                .textureIndex   = FULL_OPAQUE
            }
        );

        //? TEXT
        DrawTextCentered(renderGraph, rect, str, STRLEN_0 - 1);

        return isMouseInside && isMouseReleased;
    }

    template<u32 STRLEN_0>
    bool DrawInputField(RenderGraph& renderGraph, InputField& inputField, const char(&str)[STRLEN_0], const utils::Rect<float>& pRect, const Window& wnd)
    {
        return DrawInputField(renderGraph, inputField, str, { wnd.rect.x + pRect.x, wnd.rect.y + wnd.BAR_H + pRect.y, pRect.w, pRect.h });
    }

    template<u32 STRLEN_0>
    bool DrawInputField(
        RenderGraph& renderGraph,
        InputField& inputField,
        const char(&str)[STRLEN_0],
        const utils::Rect<float>& rect = { 0, 0, 64, 24 })
    {
        const auto STRLEN = STRLEN_0 - 1; //don't consider \0 for rendering
        const auto TOTAL_STR_W  = (STRLEN_0 + 1) * LETTER_SPACE; //not sure why strlen0 works (+1)

        const utils::Rect<float> inputRect { rect.x + TOTAL_STR_W, rect.y, rect.w - TOTAL_STR_W, rect.h };
        const utils::Rect<float> labelRect { rect.x, rect.y, TOTAL_STR_W, rect.h };
        const bool isMouseOnInput = utils::IsPointInsideRect(wnd::mouse_client_x, wnd::mouse_client_y, inputRect);
        const bool isMousePressed = wnd::CheckEvent(wnd::EventType::Mouse_Left, wnd::EventState::Pressed);
       

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

        //? INPUT FIELD
        renderGraph.ui_ubo.AppendData(
            rendering::UI_UniformData { 
                .rect           = inputRect, 
                .colorIndex     = isMouseOnInput ? BLACK4 : BLACK3,
                .textureIndex   = FULL_OPAQUE,
            }
        );

        //? LABEL
        DrawTextCentered(renderGraph, labelRect, str, STRLEN_0 - 1);

        //? INPUT
        DrawTextCentered(renderGraph, inputRect, inputField.input.dataPtr, inputField.input.Length());
        
        return false;
    }

    inline void DrawConsole(RenderGraph& renderGraph)
    {
        static Window wnd {
            .rect   = { 8, wnd::window_h - 8 - 100.f, wnd::window_w * 0.75f, 100 },
            .limits = { 100, 100, 800, 300 },
            .title  = "Console",
        };
        static InputField inputField {};

        DrawWindow(renderGraph, wnd);

        //? INPUT
        const utils::Rect<float> inputRect { 
            wnd.rect.x, 
            wnd.rect.y + wnd.rect.h - 20  , 
            wnd.rect.w - 8, 
            20 
        };
        auto input = ">>";
        DrawText(renderGraph, inputRect.x, inputRect.y, input, (u32)strlen(input));

        const bool isMouseOnInput = utils::IsPointInsideRect(wnd::mouse_client_x, wnd::mouse_client_y, inputRect);
        const bool isMousePressed = wnd::CheckEvent(wnd::EventType::Mouse_Left, wnd::EventState::Pressed);

        if (isMouseOnInput && isMousePressed) {
            inputField.isActive = true;
        }
        if (!isMouseOnInput && isMousePressed) {
            inputField.isActive = false; //probably does not cover all cases
        }
        if (inputField.isActive) {
            if (const auto* ev = wnd::CheckEvent(wnd::EventType::Keyboard_ASCII, wnd::EventState::Pressed)){
                if (ev->ascii == '\b')
                    inputField.str.Pop();
                else if (ev->ascii != '\r')
                    inputField.str.Append(ev->ascii);
            }
        }
        DrawText(renderGraph, inputRect.x + 32, inputRect.y, inputField.str.dataPtr, inputField.str.Length());
        if (const auto* ev = wnd::CheckEvent(wnd::EventType::Keyboard_ASCII, wnd::EventState::Pressed)){
            if (ev->ascii == '\r'){
                LOG(inputField.GetInt());
            }
        }
    }

    inline void DrawRenderStats(RenderGraph& renderGraph)
    {
        const auto totalInstCount = renderGraph.default_ubo.data.count;
        const auto drawCount      = renderGraph.default_ubo.groups.Count();
        const auto totalUICount   = renderGraph.ui_ubo.data.count;

        char ch_totalInstCount [] = "inst count:     ";
        char ch_drawCount []      = "draw calls:     ";
        char ch_totalUICount []   = "ui count:       ";

        std::to_chars(ch_totalInstCount + 12, ch_totalInstCount+16, totalInstCount);
        std::to_chars(ch_drawCount + 12, ch_drawCount+16, drawCount);
        std::to_chars(ch_totalUICount +12, ch_totalUICount+16, totalUICount);

        DrawText(renderGraph, 8, 20+8+ 0, ch_totalInstCount, 16);
        DrawText(renderGraph, 8, 20+8+12, ch_drawCount, 16);
        DrawText(renderGraph, 8, 20+8+24, ch_totalUICount, 16);

    }

    inline void DrawCameraPos(RenderGraph& renderGraph, const Camera& camera)
    {
        using namespace math;
        char ch_camera [100] = "camera:";
        std::to_chars(ch_camera +  8, ch_camera + 20, camera.pos[X]);
        std::to_chars(ch_camera + 22, ch_camera + 34, camera.pos[Y]);
        std::to_chars(ch_camera + 36, ch_camera + 48, camera.pos[Z]);
        //std::to_chars(ch_camera +  8, ch_camera + 24, ray[X]);
        //std::to_chars(ch_camera + 26, ch_camera + 42, ray[Y]);
        //std::to_chars(ch_camera + 44, ch_camera + 60, ray[Z]);
        DrawText(renderGraph, 8, 20+8+36, ch_camera, 100);
    }

}//ns