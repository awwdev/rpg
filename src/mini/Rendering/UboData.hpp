//https://github.com/awwdev
#pragma once
#include "mini/Math/Matrix.hpp"
#include "mini/box/Array.hpp"

namespace mini
{
    namespace rendering 
    {
        struct UniformData_Text
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

        struct UniformData_Default
        {
            math::Mat4f transform;
        };

        template<class DATA_T, u32 N>
        struct UniformDataGroups
        {
             //1 group == 1 inst type (1 draw call)
             //1 group -> N instances
            box::Array<DATA_T, N> data;  
            box::Array<UniformGroup, N> groups; 

            void Append(const box::IArray<rendering::UniformData_Default>& arr)
            {
                groups.Append(rendering::UniformGroup{
                    .begin = data.count, 
                    .count = arr.Count()
                });
                data.AppendArray(arr);
            }

            void Clear()
            {
                data.Clear();
                groups.Clear();
            }
        };

    }

}//ns