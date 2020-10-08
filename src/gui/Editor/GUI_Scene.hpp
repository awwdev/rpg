//https://github.com/awwdev

#include "gui/Widgets/Widget_Window.hpp"
#include "gui/Widgets/Widget_List.hpp"

#include "ecs/EntityID.hpp"

namespace rpg::gui {

struct GUI_Scene
{
    Widget_Window wnd 
    {
        .title = "Scene",
        .rect  = { 0, 0, 300, 500 }
    };

    Widget_List<ecs::ID> entities
    {
        .listName = "Entities"

    };

    void Update(gpu::RenderData& renderData)
    {
        wnd.Update(renderData);
        entities.Update(renderData, wnd);
    }
};

}//ns