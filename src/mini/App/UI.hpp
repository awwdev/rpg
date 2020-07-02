//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/RenderGraph/IRenderer.hpp"
#include "mini/Window/AppEvents.hpp"
#include "mini/ECS/ComponentArray.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::app::ui
{
    template<u32 STRLEN_0>
    bool DrawButton(rendergraph::RenderGraph& renderGraph, const char(&str)[STRLEN_0], const utils::Rect<int>& rect)
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
                .offset         = { (float)rect.x, (float)rect.y, 0}, 
                .size           = { (float)rect.w, (float)rect.h },
                .colorIndex     = btnColorIdx,
                .textureIndex   = 95 //this is full opaque
            }
        );

        //? TEXT
        constexpr auto STRLEN = STRLEN_0 - 1; //don't consider \0 for rendering
        constexpr auto LETTER_SIZE = 16; //monospace
        constexpr auto TOTAL_STR_W = STRLEN * LETTER_SIZE;

        const auto str_x = rect.x + rect.w * 0.5f - TOTAL_STR_W * 0.5f;
        const auto str_y = rect.y + rect.h * 0.5f - LETTER_SIZE * 0.5f;

        for(auto i = 0; i < STRLEN; ++i) {
            renderGraph.uboText.Append(
                rendergraph::UniformData_Text { 
                    .offset         = { (float) str_x + LETTER_SIZE * i, str_y, 0 }, 
                    .size           = { LETTER_SIZE, LETTER_SIZE },
                    .colorIndex     = (uint32_t)0,
                    .textureIndex   = (uint32_t)str[i] - 32 //ascii "text offset"
                }
            );
        }

        return isMouseInside && isMouseReleased;
    }


//using namespace rendergraph;
//UniformData_Text arr [10]; //same size as str
//auto fpsStr = std::to_string(dt::fps);
//for(auto i = 0; i < fpsStr.size(); ++i){
//    arr[i] = UniformData_Text{ { 32 + 32.f*i, 32.f, 0}, 0, (uint32_t)fpsStr[i] - 32 };
//}













    /*
    inline void UpdateFpsMonitorText(rendergraph::IRenderer& renderer, const double dt, ecs::C_UI& uiData)
    {
        static box::String<100> fpsStr; //!hacked
        static double counter = 0;
        counter += dt;

        if (counter >= 1)
        {
            counter -= 1;
            fpsStr.Clear();
            fpsStr.Set("fps: ");
            char buf[20];
            sprintf_s(buf, "%4.0f", 1/dt);
            fpsStr.Append(buf);
        }

        uiData.text.Set(fpsStr.dataPtr);
    }

    inline void ProcessComponents_UI(rendergraph::IRenderer& renderer, box::IArray<ecs::C_UI>& uiDatas)
    {
        FOR_ARRAY(uiDatas, i)
        {
            auto& uiData = uiDatas[i];

            if (uiData.type == ecs::C_UI::Label)
            {
                renderer.Add_DrawLabel(uiData.text.dataPtr, uiData.rect, uiData.rect_col);
            }
            
            if (uiData.type == ecs::C_UI::Button)
            {
                uiData.state = ecs::C_UI::Idle;
                if (utils::IsPointInsideRect(wnd::mouse_x, wnd::mouse_y, uiData.rect))
                {
                    uiData.state = ecs::C_UI::Hovered;
                    if (wnd::IsPressed(wnd::EventType::Mouse_Left)){
                        uiData.state = ecs::C_UI::Hold;
                    }
                    if (wnd::CheckEvent(wnd::EventType::Mouse_Left, wnd::EventState::Released)) {
                        uiData.state = ecs::C_UI::Released;
                    }
                }
                
                switch(uiData.state)
                {
                    case ecs::C_UI::Idle:     renderer.Add_DrawLabel(uiData.text.dataPtr, uiData.rect, utils::HighlightColor(uiData.rect_col, 0), uiData.text_col); break;
                    case ecs::C_UI::Hovered:  renderer.Add_DrawLabel(uiData.text.dataPtr, uiData.rect, utils::HighlightColor(uiData.rect_col, 20), uiData.text_col); break;
                    case ecs::C_UI::Hold:     renderer.Add_DrawLabel(uiData.text.dataPtr, uiData.rect, utils::HighlightColor(uiData.rect_col, 40), uiData.text_col); break;
                }
            }
        }
    }
    */
}//ns