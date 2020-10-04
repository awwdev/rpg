//https://github.com/awwdev

#pragma once
#include "res/Texture/TextureLoader.hpp"
#include "res/Texture/TextureArray.hpp"

namespace rpg::res {

struct Resources_Textures
{
    res2::TextureArray<16, 16, 128, 1> monospaceFont;

    void Load()
    {
        monospaceFont.LoadDirectory("res/TextureArrays/MonospaceFont");
    }
    
};

}//ns