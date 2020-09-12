//https://github.com/awwdev

#include "Debug/GUI/Widgets/Widget_Window.hpp"

namespace rpg::dbg::gui {

struct GUI_Level
{
    Widget_Window wnd 
    {
        .title = "Level",
        .rect  = { 0, 0, 300, 300 }
    };

    void Update(gpu::RenderData& renderData)
    {
        wnd.Update(renderData);

        /*
        FOR_STRING_MAP_BEGIN_CONST(ecs::PREFAB_STR_TO_ENUM, item)
            prefabList.items.Append(item.key);
        FOR_STRING_MAP_END
        */
    }
};

}//ns