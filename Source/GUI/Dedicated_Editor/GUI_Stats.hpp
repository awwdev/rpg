//https://github.com/awwdev

#include "GUI/Widgets/Widget_Window.hpp"
#include "GUI/Widgets/Widget_Table.hpp"
#include "Common/DeltaTime.hpp"

namespace rpg::gui {

struct GUI_Stats
{
    Widget_Window wnd 
    {
        .title = "Stats",
        .rect  = { (f32)wnd::glo::window_w - 300, 0, 300, 300 }
    };

    Widget_Table table;

    GUI_Stats()
    {
        auto row1 = table.table.Append();
        row1->Append("fps");
        row1->Append("0");
        auto row2 = table.table.Append();
        row2->Append("dt");
        row2->Append("0");

        row1->Append(""); //add some empty cells so stuff moves to the left side
        row1->Append("");
    }

    void Update(gpu::RenderData& renderData)
    {
        wnd.Update(renderData);

        if (dt::secondHasPassed){
            table.table[0][1] = dt::fps;
            table.table[1][1] = dt::seconds;
        }
        
        table.Update(renderData, wnd);
    }
};

}//ns