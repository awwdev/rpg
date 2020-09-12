//https://github.com/awwdev

#include "Debug/GUI/Widgets/Widget_Window.hpp"

namespace rpg::dbg::gui {

struct GUI_Shadow
{
    Widget_Window wnd 
    {
        .title = "Shadow",
        .rect  = { 0, 300, 300, 300 }
    };

    void Update(gpu::RenderData& renderData)
    {
        wnd.Update(renderData);
    }
};

}//ns