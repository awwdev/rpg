//https://github.com/awwdev

#pragma once

#include "mini/Utils/Types.hpp"
#include "mini/Box/Array.hpp"


namespace mini::box
{
    template<class DATA_T, u32 DATA_COUNT>
    struct AlignedStorage
    {
        constexpr static auto BYTE_SIZE = DATA_COUNT * sizeof(DATA_T);

        u8  data [BYTE_SIZE];
        u32 alignment = 0; //!set this via derived Renderer (eg. Vulkan)
        u32 count = 0;

        inline void Append(const DATA_T& uboData)
        {
            if (count * alignment >= BYTE_SIZE)
                ERR("overflow of aligned storage");

            new (&data[alignment * count]) DATA_T { uboData };
            ++count;
        }

        inline void Clear()
        {
            for(auto i = 0; i < count; ++i)
                ((DATA_T*)(&data[alignment * i]))->~DATA_T();
            count = 0;
        }

        inline auto CurrentSize() const 
        {
            return alignment * count;
        }

        DATA_T&       operator[](const u32 i)         { return *(DATA_T*)(&data[alignment * i]); }
        const DATA_T& operator[](const u32 i) const   { return *(DATA_T*)(&data[alignment * i]); }
    };

}//ns