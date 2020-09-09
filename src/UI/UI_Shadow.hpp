//https://github.com/awwdev

#pragma once
#include "UI/Widgets.hpp"
#include "Rendering/Sun.hpp"
#include "ECS/Prefabs/PrefabTypes.hpp"

namespace rpg::ui {

struct UI_Shadow
{
    //? WINDOW
    ui::Window wnd {
        .title = "Shadow",
        .rect = { (f32)wnd::global::window_w - 400.f, 0.f, 200.f, 300.f },
    };

    //? SUN
    Slider<f32> sunRotSlider {
        .name = "sun rot",
        .min  = 0,
        .max  = 6.28f,
    };

    //? DEPTH BIAS
    const Slider<f32> fac {
        .name  = "fac",
        .knobPos = 0.5,
        .min   = -15000.f,
        .max   =  0.f,
    };
    Slider<f32> depthBiasConstantFactor[3] { fac, fac, fac };

    const Slider<f32> slope {
        .name  = "slope",
        .knobPos = 0.5f,
        .min   =-2.f,
        .max   = 1,
    };
    Slider<f32> depthBiasSlopeFactor[3] { slope, slope, slope };

    //? CASCADES
    const Slider<f32> zoom {
        .name    = "cascadeZoom0",
        .min     = 0.0001f,
        .max     = 0.1f,
    };
    Slider<f32> cascadeZoom[3] { zoom, zoom, zoom };

    const Slider<f32> dist {
        .name    = "maxShadowDist0",
        .min     = 0,
        .max     = 50,
    };
    Slider<f32> maxShadowDist[3] { dist, dist, dist };
};

inline void DrawUI_Shadow(rendering::Sun& sun)
{
    static UI_Shadow ui {};
    DrawWindow(ui.wnd);


    //SUN
    sun.t = DrawSlider(ui.sunRotSlider, ui.wnd);

    //SHADOW ARTIFACTS
    //g_aciveRenderGraph->depthBiasConstantFactor[0] = DrawSlider(ui.depthBiasConstantFactor[0], ui.wnd);
    //g_aciveRenderGraph->depthBiasConstantFactor[1] = DrawSlider(ui.depthBiasConstantFactor[1], ui.wnd);
    //g_aciveRenderGraph->depthBiasConstantFactor[2] = DrawSlider(ui.depthBiasConstantFactor[2], ui.wnd);
    //g_aciveRenderGraph->depthBiasSlopeFactor[0]    = DrawSlider(ui.depthBiasSlopeFactor[0], ui.wnd);
    //g_aciveRenderGraph->depthBiasSlopeFactor[1]    = DrawSlider(ui.depthBiasSlopeFactor[1], ui.wnd);
    //g_aciveRenderGraph->depthBiasSlopeFactor[2]    = DrawSlider(ui.depthBiasSlopeFactor[2], ui.wnd);

    //CACADES
    //overwritten currently
    g_aciveRenderGraph->cascadeZoom[0]   = DrawSlider(ui.cascadeZoom[0], ui.wnd);
    g_aciveRenderGraph->cascadeZoom[1]   = DrawSlider(ui.cascadeZoom[1], ui.wnd);
    g_aciveRenderGraph->cascadeZoom[2]   = DrawSlider(ui.cascadeZoom[2], ui.wnd);
    g_aciveRenderGraph->maxShadowDist[0] = DrawSlider(ui.maxShadowDist[0], ui.wnd);
    g_aciveRenderGraph->maxShadowDist[1] = DrawSlider(ui.maxShadowDist[1], ui.wnd);
    g_aciveRenderGraph->maxShadowDist[2] = DrawSlider(ui.maxShadowDist[2], ui.wnd);
}

}//ns