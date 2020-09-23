//https://github.com/awwdev

#pragma once
#include "res/Models/Models.hpp"
#include "res/Textures/Textures.hpp"

namespace rpg::res {

struct Resources
{
    Textures textures;
    Models   models;

    void Load()
    {
        textures.Load();
        models.Load();
    }
};

}//ns