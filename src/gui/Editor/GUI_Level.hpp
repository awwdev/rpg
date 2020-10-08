//https://github.com/awwdev

#include "gui/Widgets/Widget_Window.hpp"
#include "gui/Widgets/Widget_List.hpp"
#include "gui/Widgets/Widget_Table.hpp"
#include "gui/Widgets/Widget_Checkbox.hpp"
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

    void Init(res::Resources_Prefabs const& resPrefabs)
    {
        for(idx_t prefabIdx = 0; prefabIdx < (idx_t)res::PrefabEnum::ENUM_END; ++prefabIdx)
        {
            dbg::Assert(res::PREFAB_ENUM_TO_STR.Contains(prefabIdx), "prefab missing");
            auto const prefabStr = res::PREFAB_ENUM_TO_STR.Get(prefabIdx);
            auto* item = prefabList.AddItem(prefabStr.Data(), prefabStr.Length());
        }

        {
            auto& row = table.table.Append();
            row.Append("mode");
            row.Append("");
        }
    }

    void Update(gpu::RenderData& renderData, res::Resources& res)
    {
        table.table[0][1] = [&] {
            switch(res.terrain.terrain.settings.mode) {
                case res::EditMode::VertexGrab:     return "VertexGrab";
                case res::EditMode::VertexPaint:    return "VertexPaint";
                case res::EditMode::PropPlacement:  return "PrefabPlacement";
                default: dbg::Assert(false, "EditMode missing"); return "";
            }
        }();

        wnd         .Update(renderData);
        table       .Update(renderData, wnd);
        prefabList  .Update(renderData, wnd);
        checkbox    .Update(renderData, wnd);

        res.terrain.terrain.settings.prefabEnum = (res::PrefabEnum) prefabList.activeIdx;
        renderData.general.enableTerrainWire = checkbox.isChecked;
    }
};

}//ns