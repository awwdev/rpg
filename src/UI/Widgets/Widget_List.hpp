//https://github.com/awwdev

#pragma once
#include "UI/Widgets/Widget_Common.hpp"
#include "UI/Widgets/Widget_Window.hpp"

namespace rpg::dbg::gui {

template<idx_t ITEM_COUNT_MAX>
struct Widget_List
{
    box::String<30> name;
    use::Rect<f32>  rect;
    f32 maxHeight = rect.h; //internal use mostly
    f32 scroll = 0;

    static constexpr auto NO_ACTIVE_INDEX = ITEM_COUNT_MAX;
    box::Array<box::String<30>, ITEM_COUNT_MAX> items;
    idx_t activeIdx = NO_ACTIVE_INDEX;

    void Update(gpu::RenderData& renderData)
    {
        using namespace wnd;

        const use::Rect<f32> back { rect.x, rect.y + LINE_HEIGHT, rect.w, rect.h - LINE_HEIGHT };
        const bool isMouseOnList = use::IsPointInsideRect(glo::mouse_wx, glo::mouse_wy, back);

        AddText(renderData, name, rect.x, rect.y); 
        AddRect(renderData, back, Colors::BLACK2_ALPHA);

        idx_t idx = 0;
        for(; idx < items.count; ++idx)
        {
            const use::Rect<f32> itemRect {
                back.x + PADDING,
                back.y + PADDING + idx * LINE_HEIGHT,
                back.w - PADDING * 2,
                LINE_HEIGHT
            };

            const bool isMouseOnItem = use::IsPointInsideRect(glo::mouse_wx, glo::mouse_wy, itemRect);
            if (isMouseOnList && isMouseOnItem){
                if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Pressed>())
                    activeIdx = idx;
            }

            if (activeIdx == idx)
                AddRect(renderData, itemRect, Colors::BLACK2_ALPHA);
            
            auto& str = items[idx];
            AddText(renderData, items[idx], itemRect.x, itemRect.y, isMouseOnItem ? Colors::GREEN : Colors::WHITE);
        }
    }

    void Update(gpu::RenderData& renderData, Widget_Window& wnd)
    {
        wnd.CalculateRow(rect, maxHeight);
        Update(renderData);
    }
};

}//ns