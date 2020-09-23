//https://github.com/awwdev

#pragma once
#include "com/Container/Array.hpp"
#include "com/Matrix.hpp"

namespace rpg::gpu {

struct RenderData_GUI
{
    static constexpr uint32_t UBO_TEXT_MAX = 1'000;

    struct UBO_ColorTable
    {
        alignas(256) com::Vec4f colors [9]; //how to get color table count
    };

    struct UBO_Text
    {
        alignas(16) com::Rect<f32> rect;
        alignas(16) struct 
        {
            u32 colIdx;
            u32 texIdx;
        } 
        visuals;
    };

    //? DATA
    com::Array<UBO_Text, UBO_TEXT_MAX> uboText;

    void Clear()
    {
        uboText.Clear();
    }
};

}//ns