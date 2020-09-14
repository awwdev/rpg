//https://github.com/awwdev

#pragma once
#include "GUI/Widgets/Widget_Window.hpp"

namespace rpg::gui {

template<idx_t ITEM_COUNT_MAX>
struct Widget_List
{
    com::String<30> name;
    com::Rect<f32>  rect;
    f32 maxHeight = rect.h;
    f32 scroll = 0;

    static constexpr auto NO_ACTIVE_INDEX = ITEM_COUNT_MAX;
    com::Array<com::String<30>, ITEM_COUNT_MAX> items;
    idx_t activeIdx = NO_ACTIVE_INDEX;

    void Update(gpu::RenderData& renderData)
    {
        const com::Rect<f32> back { rect.x, rect.y + LINE_HEIGHT, rect.w, rect.h - LINE_HEIGHT };
        const bool isMouseOnList = com::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, back);

        AddText(renderData, name, rect.x, rect.y); 
        AddRect(renderData, back, Colors::Black2_Alpha);

        if (isMouseOnList && wnd::glo::mouse_scroll_delta != 0){
            scroll += wnd::glo::mouse_scroll_delta < 0 ? +1 : -1;
            com::Clamp(scroll, 0, items.count - 1);
        }

        for(idx_t idx = scroll; idx < items.count; ++idx)
        {
            const com::Rect<f32> itemRect {
                back.x + PADDING,
                back.y + PADDING + (idx - scroll) * LINE_HEIGHT,
                back.w - PADDING * 2,
                LINE_HEIGHT
            };

            //max item display vertical
            if (itemRect.y + itemRect.h > rect.y + rect.h)
                break;

            const bool isMouseOnItem = isMouseOnList && com::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, itemRect);
            if (isMouseOnItem && wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Pressed>())
                activeIdx = idx;

            if (activeIdx == idx)
                AddRect(renderData, itemRect, Colors::Black2_Alpha);
            
            AddText(renderData, items[idx], itemRect.x, itemRect.y, isMouseOnItem ? Colors::Green : Colors::White);
        }
    }

    void Update(gpu::RenderData& renderData, Widget_Window& wnd)
    {
        wnd.CalculateRow(rect, maxHeight);
        Update(renderData);
    }
};

}//ns