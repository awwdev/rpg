//https://github.com/awwdev

#pragma once
#include "gui/Widgets/Widget_Window.hpp"
#include "com/mem/Allocator.hpp"
#include <functional>

namespace rpg::gui {

//allows nesting
template<class IDX_T = idx_t>
struct Widget_List
{
    using ListString = com::String<50>;

    ListString      listName;
    com::Rectf      rect;
    f32 maxHeight   = 100;
    f32 scroll      = 0;
    IDX_T activeIdx = {};
    
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
    com::Array<com::mem::BlockPtr<Item>, 100> topLevelItems;

    auto AddItem(chars_t str, idx_t const strLen, Item *const parent = nullptr) -> Item*
    {   
        auto& blockPtr = parent ? parent->subItems.Append() : topLevelItems.Append();
        com::mem::ClaimBlock(blockPtr, str, strLen);
        return blockPtr.ptr;
    }


    void Update(gpu::RenderData& renderData)
    {
        const com::Rectf back { rect.x, rect.y + LINE_HEIGHT, rect.width, rect.height - LINE_HEIGHT };
        const bool isMouseOnList = com::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, back);

        AddText(renderData, listName, rect.x, rect.y); 
        AddRectOutline(renderData, back, Colors::Black2_Alpha);

        if (isMouseOnList && wnd::glo::mouse_scroll_delta != 0){
            scroll += wnd::glo::mouse_scroll_delta < 0 ? +1 : -1;
            com::Clamp(scroll, 0, topLevelItems.Count() - 1);
            if (topLevelItems.Empty())
                scroll = 0;
        }

        //? DRAW (SUB) ITEM FN
        float intend = -1;
        std::function<void(idx_t&, Item const&)> drawItemFn = [&](idx_t& idx, Item const& item)
        {
            intend++;

            const com::Rectf itemRect {
                back.x + PADDING,
                back.y + PADDING + (idx - scroll) * LINE_HEIGHT,
                back.width - PADDING * 2,
                LINE_HEIGHT
            };

            //max item display vertical
            if (itemRect.y + itemRect.height > rect.y + rect.height)
                return;

            const bool isMouseOnItem = isMouseOnList && com::IsPointInsideRect(wnd::glo::mouse_wx, wnd::glo::mouse_wy, itemRect);
            if (isMouseOnItem && wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Pressed>())
                activeIdx = (IDX_T) idx;

            if (activeIdx == (IDX_T) idx) {
                AddRect(renderData, itemRect, Colors::Black2_Alpha);
            }
            
            AddText(renderData, item.itemName, itemRect.x + intend*4, itemRect.y, isMouseOnItem ? Colors::Green : Colors::White);

            FOR_ARRAY(item.subItems, subIdx)
            {
                ++idx;
                auto& ptrItem = item.subItems[subIdx];
                drawItemFn(idx, *ptrItem);
            }

            intend--;
        };

        //? iterate top level
        for(idx_t idx = scroll; idx < topLevelItems.Count(); ++idx) {
            auto& ptrItem = topLevelItems[idx];
            drawItemFn(idx, *ptrItem);
        }
    }

    void Update(gpu::RenderData& renderData, Widget_Window& wnd)
    {
        if (wnd.isClosed)
            return;

        wnd.CalculateRow(rect, maxHeight);
        Update(renderData);
    }
};

}//ns