//https://github.com/awwdev

#include "gui/Widgets/Widget_Window.hpp"
#include "gui/Widgets/Widget_List.hpp"
#include "gui/Widgets/Widget_Table.hpp"

#include "res/_Old/CpuResources.hpp"

namespace rpg::gui {

struct GUI_Level
{
    Widget_Window wnd 
    {
        .title  = "Level",
        .rect   = { 0, 0, 300, 300 },
        .limits = { 128, 128, f32max, f32max }
    };

    Widget_List<10> prefabList
    {
        .name = "prefab list",
        .maxHeight = 100
    };

    Widget_Table table;

    GUI_Level()
    {
        //FOR_STRING_MAP_BEGIN_CONST(ecs::PREFAB_STR_TO_ENUM, item)
        //    prefabList.items.Append(item.key);
        //FOR_STRING_MAP_END

        {
            auto& row = table.table.Append();
            row.Append("mode");
            row.Append("");
        }
    }

    void Update(gpu::RenderData& renderData, res::CpuResources& cpuRes)
    {
        table.table[0][1] = [&] {
            switch(cpuRes.terrain.settings.mode) {
                case res::EditMode::VertexGrab:     return "VertexGrab";
                case res::EditMode::VertexPaint:    return "VertexPaint";
                case res::EditMode::PropPlacement:  return "PrefabPlacement";
                default: dbg::Assert(false, "EditMode missing"); return "";
            }
        }();

        wnd         .Update(renderData);
        table       .Update(renderData, wnd);
        prefabList  .Update(renderData, wnd);

        //cpuRes.terrain.settings.prefabType = (ecs::PrefabType) prefabList.activeIdx;
        
    }
};

}//ns