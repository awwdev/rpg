//https://github.com/awwdev

#pragma once
#include "UI/Widgets.hpp"

#include <charconv>

namespace rpg::ui {

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

    statsWindow.UpdateInputMode();
}

}//ns