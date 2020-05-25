#pragma once

namespace mini
{
    template<typename VAL1, typename VAL2>
    inline auto Max(const VAL1& val1, const VAL2& val2)
    {
        return (val1 > val2) ? val1 : val2;
    }

}//ns