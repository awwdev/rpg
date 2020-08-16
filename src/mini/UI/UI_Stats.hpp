//https://github.com/awwdev

#pragma once
#include "mini/Window/WindowEvents.hpp"
#include "mini/Box/String.hpp"
#include "mini/App/InputMode.hpp"
#include "mini/Rendering/RenderGraph.hpp"
#include "mini/Resources/Terrain/Terrain.hpp"
#include "mini/UI/UI.hpp"
#include "mini/UI/UI_Terrain.hpp"
#include "mini/UI/UI_Stats.hpp"
#include "mini/Rendering/Sun.hpp"

#include <charconv>

namespace mini::ui {

void DrawUI_Stats()
{
    static ui::Window statsWindow {
        .title = "Stats",
        .rect = { 0, 0.f, 100.f, 100.f },
    };
    ui::DrawWindow(statsWindow);

    static box::String<20> fpsStr;
    if (dt::secondHasPassed) {
        fpsStr.Clear();
        fpsStr.Append("fps: ");
        fpsStr.Append(dt::fps);
    }
    static box::String<20> dtStr;
    if (dt::secondHasPassed) {
        dtStr.Clear();
        dtStr.Append("dt: ");
        dtStr.Append(dt::seconds);
    }

    ui::DrawText(fpsStr, statsWindow); 
    ui::DrawText(dtStr, statsWindow); 
}

}//ns