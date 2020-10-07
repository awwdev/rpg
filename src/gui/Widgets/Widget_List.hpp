//https://github.com/awwdev

#pragma once
#include "gui/Widgets/Widget_Window.hpp"
#include "com/mem/Allocator.hpp"

namespace rpg::gui {

template<idx_t ITEM_COUNT_MAX, idx_t NODE_COUNT_MAX = 100>
struct Widget_List
{
    using ListString = com::String<50>;

    ListString      listName;
    com::Rectf      rect;
    f32 maxHeight   = 100;
    f32 scroll      = 0;
    idx_t activeIdx = 0;
    
    struct Item
    {
        Item() = default;
        Item(chars_t itemStr, idx_t const itemStrLen)
            : itemName { itemStr, itemStrLen } //so forwarding will work
        { }

        ListString itemName;
        com::Array<com::mem::BlockPtr<Item>, 20> subItems;
        bool isToggled = false; 
    };
    com::Array<com::mem::BlockPtr<Item>, 100> items;

    auto AddItem(chars_t str, idx_t const strLen, idx_t const idx = 0) -> idx_t
    {   
        auto& ptr = items.Append();
        com::mem::ClaimBlock(ptr, str, strLen);
        return items.Count() - 1;
    }

    void Update(gpu::RenderData& renderData)
    {
        /*
        const com::Rectf back { rect.x, rect.y + LINE_HEIGHT, rect.width, rect.height - LINE_HEIGHT };
        const bool isMouseOnList = com::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, back);

        AddText(renderData, listName, rect.x, rect.y); 
        AddRect(renderData, back, Colors::Black2_Alpha);

        if (isMouseOnList && wnd::glo::mouse_scroll_delta != 0){
            scroll += wnd::glo::mouse_scroll_delta < 0 ? +1 : -1;
            com::Clamp(scroll, 0, items.Count() - 1);
        }

        for(idx_t idx = scroll; idx < items.Count(); ++idx)
        {
            const com::Rectf itemRect {
                back.x + PADDING,
                back.y + PADDING + (idx - scroll) * LINE_HEIGHT,
                back.width - PADDING * 2,
                LINE_HEIGHT
            };

            //max item display vertical
            if (itemRect.y + itemRect.height > rect.y + rect.height)
                break;

            const bool isMouseOnItem = isMouseOnList && com::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, itemRect);
            if (isMouseOnItem && wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Pressed>())
                activeIdx = idx;

            if (activeIdx == idx)
                AddRect(renderData, itemRect, Colors::Black2_Alpha);
            
            AddText(renderData, items[idx], itemRect.x, itemRect.y, isMouseOnItem ? Colors::Green : Colors::White);
        }
        */
    }

    void Update(gpu::RenderData& renderData, Widget_Window& wnd)
    {
        wnd.CalculateRow(rect, maxHeight);
        Update(renderData);
    }
};

}//ns