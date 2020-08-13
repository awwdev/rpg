//https://github.com/awwdev

#pragma once
#include "mini/Window/WindowEvents.hpp"
#include "mini/Box/String.hpp"
#include "mini/App/InputMode.hpp"
#include "mini/Rendering/RenderGraph.hpp"
#include "mini/Resources/Terrain/Terrain.hpp"
#include "mini/Rendering/UI.hpp"

#include <charconv>

namespace mini::ui {

template<
    auto QUAD_COUNT, 
    auto QUAD_LEN, 
    auto QUADRANT_COUNT>
inline void DrawTerrainData(
    rendering::RenderGraph& renderGraph, 
    const res::Terrain<QUAD_COUNT, QUAD_LEN, QUADRANT_COUNT>& terrain)
{
    static Window wnd { //static makes sure we can use valid window w
        .rect = { wnd::global::window_w-100.f, 0, 100, 100 },
        .title = "Terrain",
    }; 

    DrawWindow(renderGraph, wnd);
    wnd.ResetLines();

    const char str1 [] = "Terrain";
    const char str2 [] = "Some Data";
    const char str3 [] = "Even More Data";
    DrawText(&wnd, str1, sizeof(str1), renderGraph, ui::RED);
    DrawText(&wnd, str2, sizeof(str2), renderGraph);
    DrawText(&wnd, str3, sizeof(str3), renderGraph);
}

}//ns