//https://github.com/awwdev

#pragma once
#include "res/Texture/TextureLoader.hpp"
#include "res/_Old/TextureArray.hpp"

namespace rpg::res {

struct Resources_Textures
{
    TextureArray<128, 16, 16, 1> monospaceFont;

    void Load()
    {
        monospaceFont.LoadArray("res/TextureArrays/MonospaceFont"); //RLE would be nice
    }
    
};

}//ns