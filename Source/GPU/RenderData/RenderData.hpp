//https://github.com/awwdev

#pragma once
#include "GPU/RenderData/RenderData_Shadow.hpp"
#include "GPU/RenderData/RenderData_General.hpp"
#include "GPU/RenderData/RenderData_Post.hpp"
#include "GPU/RenderData/RenderData_GUI.hpp"

namespace rpg::gpu {

struct RenderData
{
    RenderData_Shadow   shadow;
    RenderData_General  general;
    RenderData_Post     post;
    RenderData_GUI      gui;

    struct {
        idx_t uboData_GUI_Text_previousVertCount;
        idx_t vboData_post_previousVertCount;
        idx_t vboData_general_vertCount; //updated on state because vert data comes from multiple places (and is not constructed here)
    } debugInfo;    

    void Clear()
    {
        debugInfo.uboData_GUI_Text_previousVertCount = gui.uboText.Count() * 6;
        debugInfo.vboData_post_previousVertCount     = post.vboBlur.Count() + 3; //+ fullscreen triangle

        //clear stuff, that has multiple sources that are going to append
        shadow  .Clear();
        general .Clear();
        post    .Clear();
        gui     .Clear();
    }
};

}//ns

















namespace rpg::gpu {
constexpr u32 UI_VBO_BLUR_COUNT = 100; 
constexpr u32 DEFAULT_UBO_MAX_COUNT = 1'000;
constexpr u32 DEFAULT_VERTEX_MAX_COUNT = 5000;
constexpr u32 TERRAIN_VERTEX_MAX_COUNT = 1'000'000;
}//ns

//keep rendergraph intact until all rendering (since it will be referenced on draw too)
/*
struct RenderData
{
    UniformDataGroups<UI_UniformData, RenderData_GUI::UBO_GUI_TEXT_MAX, 1>  gui_ubo; //1 group only

    UniformDataGroupsMesh<Common_UniformData, DEFAULT_UBO_MAX_COUNT> common_ubo;

    com::Array<com::Post_Vertex, UI_VBO_BLUR_COUNT> gui_vbo_blur {
        //first 3 vertices used to draw stuff from previous offscreen (full screen tris)
        com::Post_Vertex{ {-1,-1 }, {0, 0}, 0},
        com::Post_Vertex{ { 3,-1 }, {2, 0}, 0},
        com::Post_Vertex{ {-1, 3 }, {0, 2}, 0},
    };
    
    static auto constexpr CASCADE_COUNT = 3;

    f32 depthBiasConstantFactor [CASCADE_COUNT] { -2000, -3000, -10000 };
    f32 depthBiasClamp          [CASCADE_COUNT] {    0, 0, 0 };
    f32 depthBiasSlopeFactor    [CASCADE_COUNT] { -0.5, -0.5, -0.5 };

    f32 cascadeZoom     [CASCADE_COUNT] {};
    f32 maxShadowDist   [CASCADE_COUNT] {};

    void Clear()
    {
        gui_ubo.Clear();


        //common_ubo.Clear();
        //gui_vbo_blur.Clear(3); //keep first 3, they are used to draw previous offscreen
    }
};
*/