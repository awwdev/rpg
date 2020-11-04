//https://github.com/awwdev

#pragma once

namespace rpg::gpu {

union MaterialSettings
{
    struct CommonSettings
    {
    };

    struct FoliageSettings
    {
        com::Vec4f baseColor;
    };

    struct MetallicSettings
    {
    };

    struct LineSettings
    {
    };

    FoliageSettings foliage;
    CommonSettings common;
    MetallicSettings metallic;
    LineSettings lines;
};

}//ns