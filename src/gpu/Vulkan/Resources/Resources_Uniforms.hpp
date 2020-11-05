//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Resources/Uniforms/Uniforms_Instances.hpp"
#include "gpu/Vulkan/Resources/Uniforms/Uniforms_GUI.hpp"
#include "gpu/Vulkan/Resources/Uniforms/Uniforms_Post.hpp"
#include "gpu/Vulkan/Resources/Uniforms/Uniforms_Terrain.hpp"

namespace rpg::gpu::vuk {

struct Resources_Uniforms
{
    Uniforms_GUI uniformsGUI;
    Uniforms_Post uniformsPost;
    Uniforms_Terrain uniformsTerrain;
    Uniforms_Instances uniformsInstances;

    void Create()
    {
        uniformsGUI.Create();
        uniformsPost.Create();
        uniformsTerrain.Create();
        uniformsInstances.Create();
    }

    void Destroy()
    {
        uniformsGUI.Destroy();
        uniformsPost.Destroy();
        uniformsTerrain.Destroy();
        uniformsInstances.Destroy();
    }

    void Update()
    {
        uniformsGUI.Update();
        uniformsPost.Update();
        uniformsTerrain.Update();
        uniformsInstances.Update();
    }
};

}//ns