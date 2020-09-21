//https://github.com/awwdev
#pragma once
#include "Common/Matrix.hpp"
#include "Common/Container/Array.hpp"
#include "Common/Container/EnumMap.hpp"
#include "Common/Structs.hpp"
#include "Resources/MeshTypes.hpp"

namespace rpg::gpu {

//? PUST CONSTANT DATA

struct PushData_Shadow
{
    uint32_t cascadeIdx;
};

//? UBO DATA

struct UboData_GUI_Text
{
    alignas(16) com::Rect<f32> rect;
    alignas(16) struct 
    {
        u32 colIdx;
        u32 texIdx;
    } 
    visuals;
};

struct UboData_GUI_Cols
{
    alignas(256) com::Vec4f colors [9]; //how to get color table count
};

struct UboData_General_Meta
{
    alignas(64) com::Mat4f view;
    alignas(64) com::Mat4f proj;
};

struct UboData_Shadow_Sun
{
    alignas(64) com::Mat4f projView;
};

//? VERTEX DATA

struct PostVertex
{
    com::Vec2f pos;
    com::Vec2f tex;
    alignas(2) f32 blur;
};

struct GeneralVertex
{
    alignas(16) com::Vec3f pos;
    alignas(16) com::Vec3f nor;
    alignas(16) com::Vec4f col;
    alignas(16) com::Vec2f tex;
};



























struct UniformGroup
{
    u32 begin, count;
};

struct Common_UniformData
{
    alignas(64) com::Mat4f transform;
    //alignas(64) int textureIndex;
};

struct Terrain_UniformData
{
    alignas(64) com::Mat4f camProj    {};
    alignas(64) com::Mat4f camView    {};
    alignas(64) com::Mat4f sunView    {};
    com::Mat4f sunProjCasc [3]        {};
    com::Vec3f sunDir                 {};
    float cascadeFadeDist0              {};
    float cascadeFadeDist1              {};
    float cascadeFadeDist2              {};
};

template<class DATA_T, u32 DATA_COUNT, u32 GROUP_COUNT = DATA_COUNT>
struct UniformDataGroups
{
    //1 group == 1 inst type (1 draw call)
    //1 group -> N instances
    com::Array<DATA_T, DATA_COUNT> data;  
    com::EnumMap<res::MeshType::ENUM_END, UniformGroup> groups; 

    template<auto N>
    void AppendGroup(const com::Array<DATA_T, N>& arr, const res::MeshType meshType)
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
    com::Array<DATA_T, DATA_COUNT> data;  
    com::EnumMap<res::MeshType::ENUM_END, UniformGroup> groupsVertexColor; 
    com::EnumMap<res::MeshType::ENUM_END, UniformGroup> groupsTexture; 

    template<auto N>
    void AppendGroup_VertexColor(const com::Array<DATA_T, N>& arr, const res::MeshType meshType)
    {
        groupsVertexColor.Set(meshType, gpu::UniformGroup{
            .begin = data.count, 
            .count = arr.count
        });
        data.AppendArray(arr);
    }

    template<auto N>
    void AppendGroup_Texture(const com::Array<DATA_T, N>& arr, const res::MeshType meshType)
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