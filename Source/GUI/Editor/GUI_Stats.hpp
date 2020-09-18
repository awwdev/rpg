//https://github.com/awwdev

#include "GUI/Widgets/Widget_Window.hpp"
#include "GUI/Widgets/Widget_Table.hpp"
#include "Common/DeltaTime.hpp"

namespace rpg::gui {

struct GUI_Stats
{
    Widget_Window wnd 
    {
        .title  = "Stats",
        .rect   = { (f32)wnd::glo::window_w - 300, 0, 300, 300 },
        .limits = { 128, 128, f32max, f32max }
    };

    Widget_Table table;

    GUI_Stats()
    {
        {
            auto row = table.table.Append();
            row->Append("fps");
            row->Append("0");
            row->Append(""); //add some empty cells so stuff moves to the left side
            row->Append("");
        }
        {
            auto row = table.table.Append();
            row->Append("dt");
            row->Append("0");
        }
        {
            auto row = table.table.Append();
            row->Append("ui");
            row->Append("0");
        }
        {
            auto row = table.table.Append();
            row->Append("post");
            row->Append("0");
        }
    }

    void Update(gpu::RenderData& renderData)
    {
        wnd.Update(renderData);

        if (dt::secondHasPassed){
            table.table[0][1] = dt::fps;
            table.table[1][1] = dt::seconds;
            table.table[2][1] = renderData.uboData_gui_text_previousVertCount;
            table.table[2][1].Append(" verts");
            table.table[3][1] = renderData.vboData_post_previousVertCount;
            table.table[3][1].Append(" verts");
        }
        
        table.Update(renderData, wnd);
    }
};

}//ns