//https://github.com/awwdev

#include "gui/Widgets/Widget_Window.hpp"

namespace rpg::gui {

struct GUI_Scene
{
    Widget_Window wnd 
    {
        .title = "Scene",
        .rect  = { 0, 300, 300, 200 }
    };

    

    void Update(gpu::RenderData& renderData)
    {
        wnd.Update(renderData);
    }
};

}//ns