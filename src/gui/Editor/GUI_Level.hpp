//https://github.com/awwdev

#include "gui/Widgets/Widget_Window.hpp"
#include "gui/Widgets/Widget_List.hpp"
#include "gui/Widgets/Widget_Table.hpp"
#include "gui/Widgets/Widget_Checkbox.hpp"
#include "gui/Widgets/Widget_Slider.hpp"
#include "res/Resources.hpp"

namespace rpg::gui {

struct GUI_Level
{
    Widget_Window wnd 
    {
        .title  = "Level",
        .rect   = { (f32)wnd::glo::window_w - 300, 200, 300, 200 },
        .limits = { 128, 128, f32max, f32max }
    };

    Widget_List<res::PrefabEnum> prefabList
    {
        .listName = "prefab list",
        .maxHeight = 100
    };

    Widget_Table table;

    Widget_Checkbox checkbox 
    {
        .label = "Terrain Wire",
        .isChecked = true,
    };

    //TODO: color picker
    Widget_Slider<float> rSlider { .label = "R", .min = 0, .max = 1 };
    Widget_Slider<float> gSlider { .label = "G", .min = 0, .max = 1 };
    Widget_Slider<float> bSlider { .label = "B", .min = 0, .max = 1 };

    void Init(res::Resources_Prefabs const& resPrefabs)
    {
        for(idx_t prefabIdx = 0; prefabIdx < (idx_t)res::PrefabEnum::ENUM_END; ++prefabIdx)
        {
            dbg::Assert(res::PREFAB_ENUM_TO_STR.Contains(prefabIdx), "prefab missing");
            auto const prefabStr = res::PREFAB_ENUM_TO_STR.Get(prefabIdx);
            auto* item = prefabList.AddItem(prefabStr.Data(), prefabStr.Length(), (res::PrefabEnum) prefabIdx);
        }

        {
            auto& row = table.table.AppendElement();
            row.AppendElement("mode");
            row.AppendElement("");
        }
    }

    void Update(gpu::RenderData& renderData, res::Resources& res)
    {
        //table.table[0][1] = [&] {
        //    switch(res.terrain.terrain.settings.mode) {
        //        case res::EditMode::VertexGrab:     return "VertexGrab";
        //        case res::EditMode::VertexPaint:    return "VertexPaint";
        //        case res::EditMode::PrefabPlacement:  return "PrefabPlacement";
        //        default: dbg::Assert(false, "EditMode missing"); return "";
        //    }
        //}();

        wnd         .Update(renderData);

        //table       .Update(renderData, wnd);
        prefabList  .Update(renderData, wnd);
        checkbox    .Update(renderData, wnd);

        rSlider     .Update(renderData, wnd);
        gSlider     .Update(renderData, wnd);
        bSlider     .Update(renderData, wnd);

        //res.terrain.terrain.settings.prefabEnum = (res::PrefabEnum) prefabList.activeIdx;
        renderData.general.enableTerrainWire = checkbox.isChecked;

        //res.terrain.terrain.settings.vertexColor.r = rSlider.value;
        //res.terrain.terrain.settings.vertexColor.g = gSlider.value;
        //res.terrain.terrain.settings.vertexColor.b = bSlider.value;
    }
};

}//ns