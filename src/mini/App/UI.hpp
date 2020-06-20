//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Utils/Utils.hpp"
#include "mini/App/IRenderer.hpp"
#include "mini/Window/AppEvents.hpp"
#include "mini/ECS/ComponentArray.hpp"

namespace mini::app::ui
{
    enum class ButtonState { Idle, Hovered, Hold, Released, Inactive };

    //TODO: colors 
    //TODO: text position
    //TODO: border 
    //TODO: change cursor

    inline ButtonState Button(
        IRenderer& renderer,
        chars_t text, 
        const Rect<int>& rect,
        const math::Vec4f& col = { 1, 1, 1, 1 })
    {
        ButtonState state { ButtonState::Idle };

        if (IsPointInsideRect(wnd::mouse_x, wnd::mouse_y, rect))
        {
            state = ButtonState::Hovered;
            if (wnd::IsPressed(wnd::EventType::Mouse_Left)){
                state = ButtonState::Hold;
            }
            if (wnd::CheckEvent(wnd::EventType::Mouse_Left, wnd::EventState::Released)) {
                state = ButtonState::Released;
            }
        }
        
        switch(state)
        {
            case ButtonState::Idle:     renderer.Add_DrawLabel(text, rect, {0.5, 0.5, 0.5, 1}); break;
            case ButtonState::Hovered:  renderer.Add_DrawLabel(text, rect, {0.7, 0.7, 0.7, 1}); break;
            case ButtonState::Hold:     renderer.Add_DrawLabel(text, rect, {1.0, 1.0, 1.0, 1}); break;
        }

        return state;
    }

    inline void FPS_Monitor(IRenderer& renderer, const double dt)
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

        renderer.Add_DrawQuad({0, 0, 80, 16}, {1, 1, 1, 0.5});
        renderer.Add_DrawText(fpsStr.dataPtr, 2, 2, renderer.hostResources.fonts.default_font);
    }

    inline void RenderUI(IRenderer& renderer, const box::IArray<ecs::C_UI>& uiData)
    {
        FOR_ARRAY(uiData, i)
        {
            renderer.Add_DrawQuad({uiData[i].pos[Vx], uiData[i].pos[Vy], 32, 32});
        }
    }

}//ns

/*renderer.Add_DrawText(R"(Lorem ipsum dolor sit amet, 
consetetur sadipscing elitr,
sed diam nonumy eirmod tempor invidunt,
ut labore et dolore magna aliquyam erat,
sed diam voluptua. At vero eos et accusam et,
justo duo dolores et ea rebum.Stet clita kasd,
no sea takimata sanctus est Lorem ipsum dolor,
Lorem ipsum dolor sit amet, consetetur,
sed diam nonumy eirmod tempor invidunt ut labore,
sed diam voluptua. At vero eos et accusam et justo duo,
)"
, x, 0);*/