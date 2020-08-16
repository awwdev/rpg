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
    const res::Terrain<QUAD_COUNT, QUAD_LEN, QUADRANT_COUNT>& terrain,
    rendering::Sun& sun)
{
    static ui::Window terrainWnd {
        .title = "Terrain",
        .rect = { wnd::global::window_w - 100.f, 0.f, 100.f, 100.f },
    };
    ui::DrawWindow(terrainWnd);
    ui::DrawText("This is terrain!", terrainWnd);

    static ui::Slider<f32> sunRotSlider {
        .name = "sun rot",
        .min  = 0,
        .max  = 6.28f,
    };
    sun.t = ui::DrawSlider(sunRotSlider, terrainWnd);

    static ui::InputField<f32> brushInput {
        .name  = "brush size",
        .value = { "1" }
    };
    ui::DrawInputField(brushInput, terrainWnd);
}

}//ns