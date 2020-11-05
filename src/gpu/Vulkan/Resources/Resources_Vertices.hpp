//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Resources/Vertices/Vertices_Instances.hpp"
#include "gpu/Vulkan/Resources/Vertices/Vertices_GUI.hpp"
#include "gpu/Vulkan/Resources/Vertices/Vertices_Post.hpp"
#include "gpu/Vulkan/Resources/Vertices/Vertices_Terrain.hpp"

namespace rpg::gpu::vuk {

struct Resources_Vertices
{
    Vertices_GUI verticesGUI;
    Vertices_Post verticesPost;
    Vertices_Terrain verticesTerrain;
    Vertices_Instances verticesInstances;

    void Create()
    {
        verticesGUI.Create();
        verticesPost.Create();
        verticesTerrain.Create();
        verticesInstances.Create();
    }

    void Destroy()
    {
        verticesGUI.Destroy();
        verticesPost.Destroy();
        verticesTerrain.Destroy();
        verticesInstances.Destroy();
    }

    void Update()
    {
        verticesGUI.Update();
        verticesPost.Update();
        verticesTerrain.Update();
        verticesInstances.Update();
    }
};

}//ns