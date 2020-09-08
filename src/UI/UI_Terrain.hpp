//https://github.com/awwdev

#pragma once
#include "UI/Widgets.hpp"
#include "Resources/Terrain/Terrain.hpp"
#include "Rendering/Sun.hpp"
#include "ECS/Prefabs/PrefabTypes.hpp"

#include <charconv>

namespace rpg::ui {

struct UI_Terrain
{
    //? WINDOW
    Window wnd {
        .title = "Terrain",
        .rect = { wnd::global::window_w - 200.f, 0.f, 200.f, 300.f },
    };

    //? VERTEX COLOR
    Slider<f32> brushSlider {
        .name  = "brush size",
        .min   = 1.f,
        .max   = 20.f,
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

    UI_Terrain()
    {
        FOR_STRING_MAP_BEGIN_CONST(ecs::PREFAB_STR_TO_ENUM, item)
            prefabList.items.Append(item.key);
        FOR_STRING_MAP_END
    }
};

template<auto QUAD_COUNT, auto QUAD_LEN, auto QUADRANT_COUNT>
inline void DrawUI_Terrain(res::Terrain<QUAD_COUNT, QUAD_LEN, QUADRANT_COUNT>& terrain)
{
    //using TERRAIN_T = res::Terrain<QUAD_COUNT, QUAD_LEN, QUADRANT_COUNT>;

    static UI_Terrain ui {};

    //WINDOW
    DrawWindow(ui.wnd);
    ui.wnd.UpdateInputMode();

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
    if (DrawList(ui.prefabList, ui.wnd)){
        const auto& str  = ui.prefabList.items[ui.prefabList.activeIndex];
        const auto* type = ecs::PREFAB_STR_TO_ENUM.GetOptional(str.data);
        terrain.settings.prefabType = *type;
    }

}

}//ns