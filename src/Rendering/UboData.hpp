//https://github.com/awwdev
#pragma once
#include "Utils/Matrix.hpp"
#include "box/Array.hpp"
#include "Utils/Structs.hpp"

namespace rpg::gpu {

struct UI_UniformData
{
    use::Rect<f32> rect;
    u32 colorIndex;
    u32 textureIndex;
    u32 padding1;
    u32 padding2;
};

struct UniformGroup
{
    u32 begin, count;
};

struct Default_UniformData
{
    alignas(64) use::Mat4f transform;
    //alignas(64) int textureIndex;
};

struct Terrain_UniformData
{
    alignas(64) use::Mat4f camProj    {};
    alignas(64) use::Mat4f camView    {};
    alignas(64) use::Mat4f sunView    {};
    use::Mat4f sunProjCasc [3]        {};
    use::Vec3f sunDir                 {};
    float cascadeFadeDist0              {};
    float cascadeFadeDist1              {};
    float cascadeFadeDist2              {};
};

template<class DATA_T, u32 DATA_COUNT, u32 GROUP_COUNT = DATA_COUNT>
struct UniformDataGroups
{
    //1 group == 1 inst type (1 draw call)
    //1 group -> N instances
    box::Array<DATA_T, DATA_COUNT> data;  
    box::EnumMap<res::MeshType::ENUM_END, UniformGroup> groups; 

    template<auto N>
    void AppendGroup(const box::Array<DATA_T, N>& arr, const res::MeshType meshType)
    {
        groups.Set(meshType, gpu::UniformGroup{
            .begin = data.count, 
            .count = arr.count
        });
        data.AppendArray(arr);
    }

    //used when there is one group only
    void AppendData(const DATA_T& element)
    {
        data.Append(element);
    }

    void Clear()
    {
        data.Clear();
        groups.Clear();
    }
};

template<class DATA_T, u32 DATA_COUNT, u32 GROUP_COUNT = DATA_COUNT>
struct UniformDataGroupsMesh
{
    box::Array<DATA_T, DATA_COUNT> data;  
    box::EnumMap<res::MeshType::ENUM_END, UniformGroup> groupsVertexColor; 
    box::EnumMap<res::MeshType::ENUM_END, UniformGroup> groupsTexture; 

    template<auto N>
    void AppendGroup_VertexColor(const box::Array<DATA_T, N>& arr, const res::MeshType meshType)
    {
        groupsVertexColor.Set(meshType, gpu::UniformGroup{
            .begin = data.count, 
            .count = arr.count
        });
        data.AppendArray(arr);
    }

    template<auto N>
    void AppendGroup_Texture(const box::Array<DATA_T, N>& arr, const res::MeshType meshType)
    {
        groupsTexture.Set(meshType, gpu::UniformGroup{
            .begin = data.count, 
            .count = arr.count
        });
        data.AppendArray(arr);
    }

    void Clear()
    {
        data.Clear();
        groupsVertexColor.Clear();
    }
};

}//NS