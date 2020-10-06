//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData_Shadow.hpp"
#include "gpu/RenderData/RenderData_General.hpp"
#include "gpu/RenderData/RenderData_Post.hpp"
#include "gpu/RenderData/RenderData_GUI.hpp"

namespace rpg::gpu {

struct RenderData
{
    RenderData_Shadow   shadow;
    RenderData_General  general;
    RenderData_Post     post;
    RenderData_GUI      gui;

    void Clear()
    {
        shadow  .Clear();
        general .Clear();
        post    .Clear();
        gui     .Clear();
    }
};

}//ns