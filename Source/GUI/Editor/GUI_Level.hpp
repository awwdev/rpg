//https://github.com/awwdev

#include "GUI/Widgets/Widget_Window.hpp"
#include "GUI/Widgets/Widget_List.hpp"

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

    GUI_Level()
    {
        FOR_STRING_MAP_BEGIN_CONST(ecs::PREFAB_STR_TO_ENUM, item)
            prefabList.items.Append(item.key);
        FOR_STRING_MAP_END
    }

    void Update(gpu::RenderData& renderData)
    {
        wnd.Update(renderData);
        prefabList.Update(renderData, wnd);
    }
};

}//ns