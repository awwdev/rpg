//https://github.com/awwdev

#include "GUI/Widgets/Widget_Window.hpp"

namespace rpg::gui {

struct GUI_Stats
{
    Widget_Window wnd 
    {
        .title = "Stats",
        .rect  = { (f32)wnd::glo::window_w - 300, 0, 300, 300 }
    };

    void Update(gpu::RenderData& renderData)
    {
        wnd.Update(renderData);
    }
};

}//ns