//https://github.com/awwdev

#include "gui/Widgets/Widget_Window.hpp"
#include "gui/Widgets/Widget_List.hpp"
#include "gui/Widgets/Widget_Table.hpp"
#include "gui/Widgets/Widget_Checkbox.hpp"
#include "gui/Widgets/Widget_Slider.hpp"
#include "gui/Widgets/Widget_Color.hpp"

#include "res/Resources.hpp"
#include "app/Editor/EditorMode.hpp"
#include "app/Editor/Editor.hpp"

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

    Widget_Color colors;

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

    void Update(gpu::RenderData& renderData, res::Resources& res, app::Editor& editor)
    {
        //render
        wnd.Update(renderData);
        table.Update(renderData, wnd);
        prefabList.Update(renderData, wnd);
        checkbox.Update(renderData, wnd);
        colors.Update(renderData, wnd);
        //update "outside world" (update the model part of the mvc)
        table.table[0][1] = app::EDIT_MODE_ENUM_TO_STR.Get(editor.editorMode);
        renderData.general.enableTerrainWire = checkbox.isChecked;
        editor.brush.color.r = colors.rSlider.value;
        editor.brush.color.g = colors.gSlider.value;
        editor.brush.color.b = colors.bSlider.value;
        editor.brush.color.a = colors.aSlider.value;
        editor.brush.prefabEnum = (res::PrefabEnum) prefabList.activeIdx;
    }
};

}//ns