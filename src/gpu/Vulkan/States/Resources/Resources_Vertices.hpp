//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/States/Resources/Vertices/Vertices_Terrain.hpp"
#include "gpu/Vulkan/States/Resources/Vertices/Vertices_Instances.hpp"
#include "gpu/Vulkan/States/Resources/Vertices/Vertices_Post.hpp"
#include "gpu/Vulkan/States/Resources/Vertices/Vertices_GUI.hpp"

namespace rpg::gpu::vuk {

struct Resources_Vertices
{
    Vertices_Terrain verticesTerrain;
    Vertices_Instances verticesInstances;
    Vertices_Post verticesPost;
    Vertices_GUI verticesGUI;

    void Create()
    {
        verticesTerrain.Create();
        verticesInstances.Create();
        verticesPost.Create();
        verticesGUI.Create();
    }

    void Destroy()
    {
        verticesGUI.Destroy();
        verticesPost.Destroy();
        verticesTerrain.Destroy();
        verticesInstances.Destroy();
    }
};

}//ns