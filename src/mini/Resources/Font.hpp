//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"

//TODO: RESOURCE MANAGER

namespace mini::res
{
    struct Font
    {
        u32 letter_w, letter_h;
        //TODO: add mapping ref
    };

    struct Fonts
    {
        Font default_font = {
            14, 18
        };
    };    

}//ns