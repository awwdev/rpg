#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Math/Vector.hpp"

namespace mini::math
{
    template<u8 Y, u8 X, class T = float, typename = IsArithmetic<T>>
    struct Mat
    {
        T cells[Y][X]; //[0][2] == y0 x2 == y*w + x

        T*       operator[](const u8 i)       { return cells[i]; } //use as [][]
        const T* operator[](const u8 i) const { return cells[i]; } 
    };

    template<class T> using Mat4 = Mat<4, 4, T>;
    template<class T> using Mat3 = Mat<3, 3, T>;

    using Mat4f = Mat4<float>;
    using Mat3f = Mat3<float>;

    ///identity

    template<class T = float>
    [[nodiscard]] auto Identity4() -> Mat4<T>
    {
        return {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };
    }

    template<class T = float>
    [[nodiscard]] auto Identity3() -> Mat3<T>
    {
        return {
            1, 0, 0,
            0, 1, 0,
            0, 0, 1,
        };
    }
    
    ///scalar

    


    ///vec 

    template<class M, class V>
    auto operator*(const Mat4<M> mat, const Vec4<V>& vec) -> Vec4<decltype(mat[0][0]*vec.x)>
    {
        return {
        
        
        };
    }

    ///normalize

    ///project

}