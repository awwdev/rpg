//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Utils/Utils.hpp"
#include "mini/App/IRenderer.hpp"
#include "mini/Window/AppEvents.hpp"
#include "mini/ECS/ComponentArray.hpp"

namespace mini::app::ui
{
    inline void UpdateFpsMonitorText(IRenderer& renderer, const double dt, ecs::C_UI& uiData)
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

    inline void ProcessComponents_UI(IRenderer& renderer, box::IArray<ecs::C_UI>& uiData)
    {
        FOR_ARRAY(uiData, i)
        {
            if (uiData[i].type == ecs::C_UI::Label)
            {
                renderer.Add_DrawQuad(uiData[i].rect);
                renderer.Add_DrawText(uiData[i].text.dataPtr, uiData[i].rect.x, uiData[i].rect.y, renderer.hostResources.fonts.default_font);
            }
            
            if (uiData[i].type == ecs::C_UI::Button)
            {
                if (IsPointInsideRect(wnd::mouse_x, wnd::mouse_y, uiData[i].rect))
                {
                    uiData[i].state = ecs::C_UI::Hovered;
                    if (wnd::IsPressed(wnd::EventType::Mouse_Left)){
                        uiData[i].state = ecs::C_UI::Hold;
                    }
                    if (wnd::CheckEvent(wnd::EventType::Mouse_Left, wnd::EventState::Released)) {
                        uiData[i].state = ecs::C_UI::Released;
                    }
                }
                
                switch(uiData[i].state)
                {
                    case ecs::C_UI::Idle:     renderer.Add_DrawLabel(uiData[i].text.dataPtr, uiData[i].rect, {0.5, 0.5, 0.5, 1}); break;
                    case ecs::C_UI::Hovered:  renderer.Add_DrawLabel(uiData[i].text.dataPtr, uiData[i].rect, {0.7, 0.7, 0.7, 1}); break;
                    case ecs::C_UI::Hold:     renderer.Add_DrawLabel(uiData[i].text.dataPtr, uiData[i].rect, {1.0, 1.0, 1.0, 1}); break;
                }
            }
        }
    }

}//ns