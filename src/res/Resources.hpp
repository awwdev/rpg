//https://github.com/awwdev

#pragma once
#include "res/Resources_Models.hpp"
#include "res/Resources_Textures.hpp"

namespace rpg::res {

struct Resources
{
    Resources_Textures textures;
    Resources_Models   models;

    void Load()
    {
        textures.Load();
        models.Load();
    }
};

}//ns