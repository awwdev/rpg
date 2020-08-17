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

template<
    auto QUAD_COUNT, 
    auto QUAD_LEN, 
    auto QUADRANT_COUNT>
inline void DrawUI_Terrain(
    res::Terrain<QUAD_COUNT, QUAD_LEN, QUADRANT_COUNT>& terrain,
    rendering::Sun& sun)
{
    using TERRAIN_T = res::Terrain<QUAD_COUNT, QUAD_LEN, QUADRANT_COUNT>;

    //WINDOW
    static Window terrainWnd {
        .title = "Terrain",
        .rect = { wnd::global::window_w - 200.f, 0.f, 200.f, 200.f },
    };
    DrawWindow(terrainWnd);
    terrainWnd.UpdateInputMode();

    //SUN
    static Slider<f32> sunRotSlider {
        .name = "sun rot",
        .min  = 0,
        .max  = 6.28f,
    };
    sun.t = DrawSlider(sunRotSlider, terrainWnd);

    //BRUSH
    static Slider<f32> brushSlider {
        .name  = "brush size",
        .min   = 1.f,
        .max   = 10.f,
    };
    terrain.editing.brushSize = DrawSlider(brushSlider, terrainWnd);

    //VERTEX MODE
    box::String<50> vertModeStr = "Vertex Mode: ";
    if (terrain.editing.mode == TERRAIN_T::Editing::VertexGrab)
        vertModeStr.Append("VertexGrab");
    if (terrain.editing.mode == TERRAIN_T::Editing::VertexPaint)
        vertModeStr.Append("VertexPaint");
    DrawText(vertModeStr, terrainWnd);

    //VERTEX COLOR
    static Slider<f32> rSlider {
        .name  = "R",
        .min   = 0.f,
        .max   = 1.f,
    };
    static Slider<f32> gSlider {
        .name  = "G",
        .min   = 0.f,
        .max   = 1.f,
    };
    static Slider<f32> bSlider {
        .name  = "B",
        .min   = 0.f,
        .max   = 1.f,
    };
    using namespace utils;
    terrain.editing.vertexColor[X] = DrawSlider(rSlider, terrainWnd);
    terrain.editing.vertexColor[Y] = DrawSlider(gSlider, terrainWnd);
    terrain.editing.vertexColor[Z] = DrawSlider(bSlider, terrainWnd);
}

}//ns

//TODO: ui list 
//TODO: env models and finally building something :)