//https://github.com/awwdev

#include "gui/Widgets/Widget_Window.hpp"
#include "gui/Widgets/Widget_List.hpp"
#include "gui/Widgets/Widget_Table.hpp"
#include "gui/Widgets/Widget_Checkbox.hpp"
#include "res/Prefab/PrefabEnum.hpp"
#include "res/Resources.hpp"

namespace rpg::gui {

struct GUI_Level
{
    Widget_Window wnd 
    {
        .title  = "Level",
        .rect   = { 0, 0, 300, 200 },
        .limits = { 128, 128, f32max, f32max }
    };

    Widget_List<10> prefabList
    {
        .name = "prefab list",
        .maxHeight = 100
    };

    Widget_Table table;

    Widget_Checkbox checkbox 
    {
        .label = "Terrain Wire" ,
        .isChecked = true,
    };


    GUI_Level()
    {
        FOR_STRING_MAP_BEGIN_CONST(res::PREFAB_STR_TO_ENUM, prefabStr)
            prefabList.items.Append(prefabStr.key);
        FOR_STRING_MAP_END

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