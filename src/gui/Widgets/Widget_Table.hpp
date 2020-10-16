//https://github.com/awwdev

#pragma once
#include "gui/Widgets/Widget_Window.hpp"
#include "com/box/Array.hpp"
#include "com/box/String.hpp"

namespace rpg::gui {

struct Widget_Table
{
    using Row = com::Array<com::String<50>, 5>;
    com::Array<Row, 30> table;
    com::Rectf rect;

    void Update(gpu::RenderData& renderData, bool const insideWindow = true)
    {
        if (insideWindow == false)
            return;

        const auto CELL_H = LINE_HEIGHT;
        const auto CELL_W = [&]{
            idx_t count = 1;
            FOR_ARRAY(table, y) {
                if (table[y].Count() > count)
                    count = table[y].Count();
            };
            return rect.width / count;
        }();

        FOR_ARRAY(table, y) 
        {
            if (y * CELL_H > rect.height)
                break;

            FOR_ARRAY(table [y], x) 
            {
                AddText(renderData, table[y][x], 
                rect.x + x * CELL_W, 
                rect.y + y * CELL_H, 
                x == 0 ? Colors::White : Colors::Orange); 
            }
        }
    }

    void Update(gpu::RenderData& renderData, Widget_Window& wnd)
    {
        if (wnd.isClosed)
            return;

        auto const maxHeight = table.Count() * LINE_HEIGHT;
        bool const insideWindow = wnd.CalculateRow(rect, maxHeight);
        Update(renderData, insideWindow);
    }
};

}//ns