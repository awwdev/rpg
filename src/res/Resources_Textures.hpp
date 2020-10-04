//https://github.com/awwdev

#pragma once
#include "res/Texture/TextureLoader.hpp"
#include "res/Texture/TextureArray.hpp"

namespace rpg::res {

struct Resources_Textures
{
    res::TextureArray<16, 16, 128, 1> monospaceFont;
    res::TextureArray<512, 512, 1, 1> foliage;

    void Load()
    {
        monospaceFont   .LoadDirectory_GreyFormat("res/TextureArrays/MonospaceFont");
        foliage         .LoadDirectory_GreyFormat("res/TextureArrays/Foliage");
    }
    
};

}//ns