//https://github.com/awwdev

#pragma once

#include "mini/Utils/Types.hpp"
#include "mini/Box/Array.hpp"


namespace mini::box
{
    template<u32 BYTE_SIZE, class UBO_DATA>
    struct AlignedStorage
    {
        u8  data [BYTE_SIZE];
        u32 alignment = 0; //!set this via derived Renderer (eg. Vulkan)
        u32 count = 0;

        inline void Append(const UBO_DATA& uboData)
        {
            new (&data[alignment * count]) UBO_DATA { uboData };
            ++count;
        }

        inline void Clear()
        {
            for(auto i = 0; i < count; ++i)
                ((UBO_DATA*)(&data[alignment * i]))->~UBO_DATA();
            count = 0;
        }

        inline auto CurrentSize() const 
        {
            return alignment * count;
        }

        UBO_DATA&       operator[](const u32 i)         { return *(UBO_DATA*)(&data[alignment * i]); }
        const UBO_DATA& operator[](const u32 i) const   { return *(UBO_DATA*)(&data[alignment * i]); }
    };

}//ns