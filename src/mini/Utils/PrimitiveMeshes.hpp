//https://github.com/awwdev

#pragma once
#include "mini/Math/Matrix.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::utils
{
    constexpr math::Vec4f GREEN = { 0, 1, 0, 1 };
    constexpr math::Vec4f BLUE  = { 0, 0, 1, 1 };
    constexpr math::Vec4f RED   = { 1, 0, 0, 1 };
    constexpr math::Vec4f WHITE = { 1, 1, 1, 1 };
    

    const utils::Common_Vertex MESH_CUBE [] {
        { { -1, 1, 1, 1 }, {}, GREEN, {} },
        { {  1, 1, 1, 1 }, {}, GREEN, {} },
        { {  1, 1,-1, 1 }, {}, GREEN, {} },
        { {  1, 1, 1, 1 }, {}, GREEN, {} },
        { {  1,-1, 1, 1 }, {}, GREEN, {} },
        { {  1,-1,-1, 1 }, {}, GREEN, {} },
        { {  1, 1,-1, 1 }, {}, RED, {} },
        { {  1,-1,-1, 1 }, {}, RED, {} },
        { { -1,-1,-1, 1 }, {}, RED, {} },
        { { -1,-1, 1, 1 }, {}, RED, {} },
        { { -1, 1, 1, 1 }, {}, RED, {} },
        { { -1, 1,-1, 1 }, {}, RED, {} },
        { {  1,-1, 1, 1 }, {}, WHITE, {} },
        { { -1,-1, 1, 1 }, {}, WHITE, {} },
        { { -1,-1,-1, 1 }, {}, WHITE, {} },
        { { -1, 1, 1, 1 }, {}, WHITE, {} },
        { { -1,-1, 1, 1 }, {}, WHITE, {} },
        { {  1,-1, 1, 1 }, {}, WHITE, {} },
        { { -1, 1, 1, 1 }, {}, GREEN, {} },
        { {  1,-1, 1, 1 }, {}, GREEN, {} },
        { {  1, 1, 1, 1 }, {}, GREEN, {} },
        { {  1,-1, 1, 1 }, {}, GREEN, {} },
        { { -1,-1,-1, 1 }, {}, GREEN, {} },
        { {  1,-1,-1, 1 }, {}, GREEN, {} },
        { { -1,-1, 1, 1 }, {}, BLUE, {} },
        { { -1, 1,-1, 1 }, {}, BLUE, {} },
        { { -1,-1,-1, 1 }, {}, BLUE, {} },
        { {  1, 1,-1, 1 }, {}, BLUE, {} },
        { { -1,-1,-1, 1 }, {}, BLUE, {} },
        { { -1, 1,-1, 1 }, {}, BLUE, {} },
        { {  1, 1, 1, 1 }, {}, GREEN, {} },
        { {  1,-1,-1, 1 }, {}, GREEN, {} },
        { {  1, 1,-1, 1 }, {}, GREEN, {} },
        { { -1, 1, 1, 1 }, {}, GREEN, {} },
        { {  1, 1,-1, 1 }, {}, GREEN, {} },
        { { -1, 1,-1, 1 }, {}, GREEN, {} },
    };

    const utils::Common_Vertex MESH_QUAD [] {
        { { -1.0f, -1.0f, 0, 1 }, {}, { GREEN }, {} },
        { {  1.0f, -1.0f, 0, 1 }, {}, { GREEN }, {} },
        { {  1.0f,  1.0f, 0, 1 }, {}, { GREEN }, {} },
        { { -1.0f, -1.0f, 0, 1 }, {}, { RED }, {} },
        { {  1.0f,  1.0f, 0, 1 }, {}, { RED }, {} },
        { { -1.0f,  1.0f, 0, 1 }, {}, { RED }, {} },
    };

    const utils::Common_Vertex MESH_TRIANGLE [] = {
        { {  0.0f, -1.0f, 0, 1 }, {}, { .8f, .2f, .2f, 1 }, {} },
        { {  1.0f,  1.0f, 0, 1 }, {}, { .8f, .2f, .2f, 1 }, {} },
        { { -1.0f,  1.0f, 0, 1 }, {}, { .8f, .2f, .2f, 1 }, {} },
    };

}//ns