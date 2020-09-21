//https://github.com/awwdev

#pragma once
#include "GPU/RenderStructs.hpp"

namespace rpg::gpu {

struct RenderData
{
    UboData_General_Meta uboData_general_meta; //camera data
    UboData_Shadow_Sun   uboData_shadow_sun;

    com::Array<UboData_GUI_Text, UBO_GUI_TEXT_MAX> uboData_gui_text;
    com::Array<PostVertex, VBO_POST_MAX> vboData_post;

    struct {
        idx_t uboData_gui_text_previousVertCount;
        idx_t vboData_post_previousVertCount;
        idx_t vboData_general_vertCount; //updated on state because vert data comes from multiple places (and is not constructed here)
    } debugInfo;    
    
    void Clear()
    {
        debugInfo.uboData_gui_text_previousVertCount = uboData_gui_text.count * 6;
        debugInfo.vboData_post_previousVertCount     = vboData_post.count + 3; //+ fullscreen triangle

        //clear stuff, that has multiple sources that are going to append
        uboData_gui_text.Clear();
        vboData_post.Clear();
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
    UniformDataGroups<UI_UniformData, UBO_GUI_TEXT_MAX, 1>  gui_ubo; //1 group only

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