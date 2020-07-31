//https://github.com/awwdev
#pragma once
#include "mini/Math/Matrix.hpp"
#include "mini/box/Array.hpp"

namespace mini
{
    namespace rendering 
    {
        struct UI_UniformData
        {
            utils::Rect<float> rect;
            uint32_t colorIndex;
            uint32_t textureIndex; 
            uint32_t padding1;
            uint32_t padding2;
        };

        struct UniformGroup
        {
            u32 begin, count;
        };

        struct Default_UniformData
        {
            math::Mat4f transform;
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

    }

}//ns