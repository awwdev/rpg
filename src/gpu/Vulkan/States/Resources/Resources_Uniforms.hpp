//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/States/Resources/Uniforms/Uniforms_Shadow.hpp"
#include "gpu/Vulkan/States/Resources/Uniforms/Uniforms_Terrain.hpp"
#include "gpu/Vulkan/States/Resources/Uniforms/Uniforms_Instances.hpp"
#include "gpu/Vulkan/States/Resources/Uniforms/Uniforms_Post.hpp"
#include "gpu/Vulkan/States/Resources/Uniforms/Uniforms_GUI.hpp"

namespace rpg::gpu::vuk {

struct Resources_Uniforms
{
    Uniforms_Shadow uniformsShadow;
    Uniforms_Terrain uniformsTerrain;
    Uniforms_Instances uniformsInstances;
    Uniforms_Post uniformsPost;
    Uniforms_GUI uniformsGUI;

    void Create()
    {
        uniformsShadow.Create();
        uniformsTerrain.Create();
        uniformsInstances.Create();
        uniformsPost.Create();
        uniformsGUI.Create();
    }

    void Destroy()
    {
        uniformsGUI.Destroy();
        uniformsPost.Destroy();
        uniformsInstances.Destroy();
        uniformsTerrain.Destroy();
        uniformsShadow.Destroy();
    }

};

}//ns