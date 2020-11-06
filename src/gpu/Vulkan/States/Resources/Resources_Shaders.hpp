//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Context.hpp"
#include "gpu/Vulkan/Wrappers/Shader.hpp"

namespace rpg::gpu::vuk {

struct Resources_Shaders
{
    VertexFragmentShader postShader;
    VertexFragmentShader guiShader;
    VertexFragmentShader shadowShader;
    //terrain
    VertexFragmentShader terrainShader;
    VertexFragmentShader terrainWireShader;
    //instances
    VertexFragmentShader generalShader;
    VertexFragmentShader foliageShader;
    VertexFragmentShader lineShader;

    void Create()
    {
        postShader       .CreateStage(ShaderStage::Vert, "res/Shaders/Post/spv/post.vert.spv");
        postShader       .CreateStage(ShaderStage::Frag, "res/Shaders/Post/spv/post.frag.spv");
        guiShader        .CreateStage(ShaderStage::Vert, "res/Shaders/GUI/spv/gui.vert.spv");
        guiShader        .CreateStage(ShaderStage::Frag, "res/Shaders/GUI/spv/gui.frag.spv");
        shadowShader     .CreateStage(ShaderStage::Vert, "res/Shaders/Shadow/spv/shadow.vert.spv");
        shadowShader     .CreateStage(ShaderStage::Frag, "res/Shaders/Shadow/spv/shadow.frag.spv");
        //terrain
        terrainShader    .CreateStage(ShaderStage::Vert, "res/Shaders/Main/Terrain/spv/terrain.vert.spv");
        terrainShader    .CreateStage(ShaderStage::Frag, "res/Shaders/Main/Terrain/spv/terrain.frag.spv");
        terrainWireShader.CreateStage(ShaderStage::Vert, "res/Shaders/Main/Terrain/spv/terrainWire.vert.spv");
        terrainWireShader.CreateStage(ShaderStage::Frag, "res/Shaders/Main/Terrain/spv/terrainWire.frag.spv");
        //instances
        generalShader    .CreateStage(ShaderStage::Vert, "res/Shaders/Main/Instances/spv/general.vert.spv");
        generalShader    .CreateStage(ShaderStage::Frag, "res/Shaders/Main/Instances/spv/general.frag.spv");
        foliageShader    .CreateStage(ShaderStage::Vert, "res/Shaders/Main/Instances/spv/foliage.vert.spv");
        foliageShader    .CreateStage(ShaderStage::Frag, "res/Shaders/Main/Instances/spv/foliage.frag.spv");
        lineShader       .CreateStage(ShaderStage::Vert, "res/Shaders/Main/Instances/spv/line.vert.spv");
        lineShader       .CreateStage(ShaderStage::Frag, "res/Shaders/Main/Instances/spv/line.frag.spv");
    }

    void Destroy()
    {
        postShader.Destroy(); 
        guiShader.Destroy(); 
        shadowShader.Destroy(); 
        //terrain
        terrainShader.Destroy();
        terrainWireShader.Destroy();
        //instances
        generalShader.Destroy();
        foliageShader.Destroy();
        lineShader.Destroy();
    }
};

}//ns