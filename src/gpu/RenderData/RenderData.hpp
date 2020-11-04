//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData_Shadow.hpp"
#include "gpu/RenderData/RenderData_Main.hpp"
#include "gpu/RenderData/RenderData_Post.hpp"
#include "gpu/RenderData/RenderData_GUI.hpp"

namespace rpg::gpu {

struct RenderData
{
    RenderData_Shadow   shadow;
    RenderData_Main     main;
    RenderData_Post     post;
    RenderData_GUI      gui;

    void Clear()
    {
        shadow  .Clear();
        main    .Clear();
        post    .Clear();
        gui     .Clear();
    }
};

}//ns