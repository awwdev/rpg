//https://github.com/awwdev

#pragma once
#include "gui/Widgets/Widget_Window.hpp"
#include "gui/Widgets/Widget_Slider.hpp"

namespace rpg::gui {

struct Widget_Color
{
    Widget_Slider<float> rSlider { .label = "R", .min = 0, .max = 1, .value = 1 };
    Widget_Slider<float> gSlider { .label = "G", .min = 0, .max = 1, .value = 1 };
    Widget_Slider<float> bSlider { .label = "B", .min = 0, .max = 1, .value = 1 };
    Widget_Slider<float> aSlider { .label = "A", .min = 0, .max = 1, .value = 1 };

    void Update(gpu::RenderData& renderData, Widget_Window& wnd)
    {
        rSlider.Update(renderData, wnd);
        gSlider.Update(renderData, wnd);
        bSlider.Update(renderData, wnd);
        aSlider.Update(renderData, wnd);
    }
};

}//ns