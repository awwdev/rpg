//https://github.com/awwdev

#pragma once
#include "UI/Widgets.hpp"
#include "Rendering/Sun.hpp"
#include "ECS/Prefabs/PrefabTypes.hpp"

namespace rpg::ui {

struct UI_Shadow
{

    ui::Window wnd {
        .title = "Shadow",
        .rect = { (f32)wnd::global::window_w - 400.f, 0.f, 200.f, 300.f },
    };

    //Slider<f32> sunRotSlider {
    //    .name = "sun rot",
    //    .min  = 0,
    //    .max  = 6.28f,
    //};

    /*
    Slider<f32> depthBiasConstantFactor {
        .name  = "fac",
        .knobPos = 0.5,
        .min   =-5000.f,
        .max   = 0.f,
    };
    Slider<f32> depthBiasClamp {
        .name  = "clamp",
        .knobPos = 0.5,
        .min   =-100.f,
        .max   = 100.f,
    };
    Slider<f32> depthBiasSlopeFactor {
        .name  = "slope",
        .knobPos = 0.5f,
        .min   =-10.f,
        .max   =  2,
    };

    //? CASCADES
    Slider<f32> cascadeZoom0 {
        .name    = "cascadeZoom0",
        .min     = 0.0001f,
        .max     = 0.1f,
    };
    Slider<f32> cascadeZoom1 {
        .name    = "cascadeZoom1",
        .min     = 0.0001f,
        .max     = 0.1f,
    };
    Slider<f32> cascadeZoom2 {
        .name    = "cascadeZoom2",
        .min     = 0.0001f,
        .max     = 0.1f,
    };
    Slider<f32> maxShadowDist0 {
        .name    = "maxShadowDist0",
        .min     = 0,
        .max     = 50,
    };
    Slider<f32> maxShadowDist1 {
        .name    = "maxShadowDist1",
        .min     = 30,
        .max     = 300,
    };
    Slider<f32> maxShadowDist2 {
        .name    = "maxShadowDist2",
        .min     = 300,
        .max     = 1000,
    };
    */
};

inline void DrawUI_Shadow(rendering::Sun&)
{
    //static UI_Shadow ui {};

    //SUN
    //sun.t = DrawSlider(ui.sunRotSlider, ui.wnd);

    //SHADOW ARTIFACTS
    //g_aciveRenderGraph->depthBiasConstantFactor = DrawSlider(ui.depthBiasConstantFactor, ui.wnd);
    //g_aciveRenderGraph->depthBiasClamp          = DrawSlider(ui.depthBiasClamp, ui.wnd);
    //g_aciveRenderGraph->depthBiasSlopeFactor    = DrawSlider(ui.depthBiasSlopeFactor, ui.wnd);

    //CACADES
    //g_aciveRenderGraph->cascadeZoom0   = DrawSlider(ui.cascadeZoom0, ui.wnd);
    //g_aciveRenderGraph->cascadeZoom1   = DrawSlider(ui.cascadeZoom1, ui.wnd);
    //g_aciveRenderGraph->cascadeZoom2   = DrawSlider(ui.cascadeZoom2, ui.wnd);
    //g_aciveRenderGraph->maxShadowDist0 = DrawSlider(ui.maxShadowDist0, ui.wnd);
    //g_aciveRenderGraph->maxShadowDist1 = DrawSlider(ui.maxShadowDist1, ui.wnd);
    //g_aciveRenderGraph->maxShadowDist2 = DrawSlider(ui.maxShadowDist2, ui.wnd);
}

}//ns