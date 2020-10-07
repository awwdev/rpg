//https://github.com/awwdev

#pragma once
#include "res/Resources_Textures.hpp"
#include "res/Resources_Terrain.hpp"
#include "res/Resources_Prefabs.hpp"
#include "res/Resources_Meshes.hpp"
#include "res/Resources_Animation.hpp"

namespace rpg::res {

struct Resources
{
    Resources_Textures  textures;
    Resources_Meshes    meshes;
    Resources_Terrain   terrain;
    Resources_Prefabs   prefabs;
    Resources_Animation animation;

    Resources() //immediate load when ctor
    {
        prefabs.Load();
        textures.Load();
        meshes.Load();
        terrain.Load();
        animation.Load();
    }
};

}//ns