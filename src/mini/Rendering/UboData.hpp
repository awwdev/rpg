//https://github.com/awwdev
#pragma once
#include "mini/Utils/Matrix.hpp"
#include "mini/box/Array.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::rendering {

struct UI_UniformData
{
    utils::Rect<f32> rect;
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
    utils::Mat4f transform;
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
        groups.Set(meshType, rendering::UniformGroup{
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

}//ns