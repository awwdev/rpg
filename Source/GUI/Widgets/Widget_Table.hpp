//https://github.com/awwdev

#pragma once
#include "GUI/Widgets/Widget_Window.hpp"
#include "Common/Container/Array.hpp"
#include "Common/Container/String.hpp"

namespace rpg::gui {

struct Widget_Table
{
    using Row = com::Array<com::String<30>, 5>;
    com::Array<Row, 30> table;
    com::Rect<f32> rect;

    void Update(gpu::RenderData& renderData)
    {
        const auto CELL_H = LINE_HEIGHT;
        const auto CELL_W = [&]{
            idx_t count = 1;
            FOR_ARRAY(table, y) {
                if (table[y].count > count)
                    count = table[y].count;
            };
            return rect.w / count;
        }();

        FOR_ARRAY(table, y) {
        FOR_ARRAY(table [y], x) {
            AddText(renderData, table[y][x], 
            rect.x + x * CELL_W, 
            rect.y + y * CELL_H, 
            x == 0 ? Colors::White : Colors::Orange ); 
        }}
    }

    void Update(gpu::RenderData& renderData, Widget_Window& wnd)
    {
        const auto maxHeight = table.count * LINE_HEIGHT;
        wnd.CalculateRow(rect, maxHeight);
        Update(renderData);
    }
};

}//ns