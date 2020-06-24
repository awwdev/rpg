//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Utils/Algorithms.hpp"
#include "mini/RenderGraph/IRenderer.hpp"
#include "mini/Window/AppEvents.hpp"
#include "mini/ECS/ComponentArray.hpp"
#include "mini/Utils/Algorithms.hpp"

namespace mini::app::ui
{
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

}//ns