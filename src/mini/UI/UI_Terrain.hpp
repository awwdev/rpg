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
#include "mini/ECS/Prefabs2.hpp"

#include <charconv>

namespace mini::ui {

struct TerrainUI
{
    Window wnd {
        .title = "Terrain",
        .rect = { wnd::global::window_w - 200.f, 0.f, 200.f, 300.f },
    };

    Slider<f32> sunRotSlider {
        .name = "sun rot",
        .min  = 0,
        .max  = 6.28f,
    };

    Slider<f32> brushSlider {
        .name  = "brush size",
        .min   = 1.f,
        .max   = 10.f,
    };

    Slider<f32> rSlider {
        .name  = "R",
        .min   = 0.f,
        .max   = 1.f,
    };
    Slider<f32> gSlider {
        .name  = "G",
        .min   = 0.f,
        .max   = 1.f,
    };
    Slider<f32> bSlider {
        .name  = "B",
        .min   = 0.f,
        .max   = 1.f,
    };

    List<box::String<20>, 10> prefabList {
        .name  = "Prefabs",
        .rect  = { 0, 0, 0, 100 },
    };

    TerrainUI()
    {
        FOR_STRING_MAP_BEGIN_CONST(ecs::PREFAB_STRINGS, item)
            prefabList.items.Append(item.key);
        FOR_STRING_MAP_END
    }
};

template<
    auto QUAD_COUNT, 
    auto QUAD_LEN, 
    auto QUADRANT_COUNT>
inline void DrawUI_Terrain(
    res::Terrain<QUAD_COUNT, QUAD_LEN, QUADRANT_COUNT>& terrain,
    rendering::Sun& sun)
{
    using TERRAIN_T = res::Terrain<QUAD_COUNT, QUAD_LEN, QUADRANT_COUNT>;

    static TerrainUI ui {};

    //WINDOW
    DrawWindow(ui.wnd);
    ui.wnd.UpdateInputMode();

    //SUN
    sun.t = DrawSlider(ui.sunRotSlider, ui.wnd);

    //BRUSH
    terrain.settings.brushSize = DrawSlider(ui.brushSlider, ui.wnd);

    //VERTEX MODE
    box::String<50> vertModeStr = "Vertex Mode: ";
    if (terrain.settings.mode == res::EditMode::VertexGrab)
        vertModeStr.Append("VertexGrab");
    if (terrain.settings.mode == res::EditMode::VertexPaint)
        vertModeStr.Append("VertexPaint");
    DrawText(vertModeStr, ui.wnd);

    //VERTEX COLOR
    using namespace utils;
    terrain.settings.vertexColor[X] = DrawSlider(ui.rSlider, ui.wnd);
    terrain.settings.vertexColor[Y] = DrawSlider(ui.gSlider, ui.wnd);
    terrain.settings.vertexColor[Z] = DrawSlider(ui.bSlider, ui.wnd);

    //LIST
    DrawList(ui.prefabList, ui.wnd);
}

}//ns