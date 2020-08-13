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

    box::String<100> str = "Mode: ";
    if (terrain.mode == std::decay_t<decltype(terrain)>::VertexGrab)
        str.Append("VertexGrab");
    if (terrain.mode == std::decay_t<decltype(terrain)>::VertexPaint)
        str.Append("VertexPaint");

    DrawText(&wnd, str.data, str.Length(), renderGraph);
}

}//ns