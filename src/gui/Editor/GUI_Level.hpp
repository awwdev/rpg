//https://github.com/awwdev

#include "gui/Widgets/Widget_Window.hpp"
#include "gui/Widgets/Widget_List.hpp"
#include "gui/Widgets/Widget_Table.hpp"
#include "gui/Widgets/Widget_Checkbox.hpp"
#include "gui/Widgets/Widget_Slider.hpp"

#include "res/Resources.hpp"
#include "app/Editor/EditorMode.hpp"

namespace rpg::gui {

struct GUI_Level
{
    Widget_Window wnd 
    {
        .title  = "Level",
        .rect   = { 0, Widget_Window::BAR_H * 2, 300, 300 },
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

    void Update(gpu::RenderData& renderData, res::Resources& res, app::EditorMode editorMode)
    {
        table.table[0][1] = app::EDIT_MODE_ENUM_TO_STR.Get(editorMode);

        wnd.Update(renderData);

        table.Update(renderData, wnd);
        prefabList.Update(renderData, wnd);
        checkbox.Update(renderData, wnd);

        //vertex colors
        rSlider.Update(renderData, wnd);
        gSlider.Update(renderData, wnd);
        bSlider.Update(renderData, wnd);

        //res.terrain.terrain.settings.prefabEnum = (res::PrefabEnum) prefabList.activeIdx;
        renderData.general.enableTerrainWire = checkbox.isChecked;
        //res.terrain.terrain.settings.vertexColor.r = rSlider.value;
        //res.terrain.terrain.settings.vertexColor.g = gSlider.value;
        //res.terrain.terrain.settings.vertexColor.b = bSlider.value;
    }
};

}//ns