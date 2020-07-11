//https://github.com/awwdev

#pragma once
#include "mini/Math/Matrix.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::utils
{
    constexpr f32 S = 1.0f;
    constexpr f32 D = 0.0f;
    constexpr math::Vec4f GREEN = { 0, 1, 0, 1 };
    constexpr math::Vec4f BLUE  = { 0, 0, 1, 1 };
    constexpr math::Vec4f RED   = { 1, 0, 0, 1 };
    constexpr math::Vec4f WHITE = { 1, 1, 1, 1 };

    const utils::Vertex MESH_CUBE [] {
        { { -1*S, 1*S, 1*S - D , 1 }, {}, GREEN, {} },
        { {  1*S, 1*S, 1*S - D , 1 }, {}, GREEN, {} },
        { {  1*S, 1*S,-1*S - D , 1 }, {}, GREEN, {} },
        { {  1*S, 1*S, 1*S - D , 1 }, {}, GREEN, {} },
        { {  1*S,-1*S, 1*S - D , 1 }, {}, GREEN, {} },
        { {  1*S,-1*S,-1*S - D , 1 }, {}, GREEN, {} },
        { {  1*S, 1*S,-1*S - D , 1 }, {}, RED, {} },
        { {  1*S,-1*S,-1*S - D , 1 }, {}, RED, {} },
        { { -1*S,-1*S,-1*S - D , 1 }, {}, RED, {} },
        { { -1*S,-1*S, 1*S - D , 1 }, {}, RED, {} },
        { { -1*S, 1*S, 1*S - D , 1 }, {}, RED, {} },
        { { -1*S, 1*S,-1*S - D , 1 }, {}, RED, {} },
        { {  1*S,-1*S, 1*S - D , 1 }, {}, WHITE, {} },
        { { -1*S,-1*S, 1*S - D , 1 }, {}, WHITE, {} },
        { { -1*S,-1*S,-1*S - D , 1 }, {}, WHITE, {} },
        { { -1*S, 1*S, 1*S - D , 1 }, {}, WHITE, {} },
        { { -1*S,-1*S, 1*S - D , 1 }, {}, WHITE, {} },
        { {  1*S,-1*S, 1*S - D , 1 }, {}, WHITE, {} },
        { { -1*S, 1*S, 1*S - D , 1 }, {}, GREEN, {} },
        { {  1*S,-1*S, 1*S - D , 1 }, {}, GREEN, {} },
        { {  1*S, 1*S, 1*S - D , 1 }, {}, GREEN, {} },
        { {  1*S,-1*S, 1*S - D , 1 }, {}, GREEN, {} },
        { { -1*S,-1*S,-1*S - D , 1 }, {}, GREEN, {} },
        { {  1*S,-1*S,-1*S - D , 1 }, {}, GREEN, {} },
        { { -1*S,-1*S, 1*S - D , 1 }, {}, BLUE, {} },
        { { -1*S, 1*S,-1*S - D , 1 }, {}, BLUE, {} },
        { { -1*S,-1*S,-1*S - D , 1 }, {}, BLUE, {} },
        { {  1*S, 1*S,-1*S - D , 1 }, {}, BLUE, {} },
        { { -1*S,-1*S,-1*S - D , 1 }, {}, BLUE, {} },
        { { -1*S, 1*S,-1*S - D , 1 }, {}, BLUE, {} },
        { {  1*S, 1*S, 1*S - D , 1 }, {}, GREEN, {} },
        { {  1*S,-1*S,-1*S - D , 1 }, {}, GREEN, {} },
        { {  1*S, 1*S,-1*S - D , 1 }, {}, GREEN, {} },
        { { -1*S, 1*S, 1*S - D , 1 }, {}, GREEN, {} },
        { {  1*S, 1*S,-1*S - D , 1 }, {}, GREEN, {} },
        { { -1*S, 1*S,-1*S - D , 1 }, {}, GREEN, {} },
    };
}//ns

/*
utils::Vertex tris [] = {
    { { -1.0f, -1.0f, +0.0f, +1.0f }, {}, { 1.f, 0.f, 0.f, 1.f }, {} },
    { { +0.0f, -1.0f, +0.0f, +1.0f }, {}, { 0.f, 1.f, 0.f, 1.f }, {} },
    { { -1.0f, +1.0f, +0.0f, +1.0f }, {}, { 0.f, 0.f, 1.f, 1.f }, {} },
};
utils::Vertex quad [] = {
    { { -0.0f, -1.0f, +0.0f, +1.0f }, {}, { 0.3f, 0.3f, 0.8f, 1.f }, {} },
    { { +1.0f, -1.0f, +0.0f, +1.0f }, {}, { 0.3f, 0.3f, 0.8f, 1.f }, {} },
    { { +1.0f, +1.0f, +0.0f, +1.0f }, {}, { 0.3f, 0.3f, 0.8f, 1.f }, {} },
    { { -0.0f, -1.0f, +0.0f, +1.0f }, {}, { 0.3f, 0.3f, 0.8f, 1.f }, {} },
    { { +1.0f, +1.0f, +0.0f, +1.0f }, {}, { 0.3f, 0.3f, 0.8f, 1.f }, {} },
    { { +0.0f, +1.0f, +0.0f, +1.0f }, {}, { 0.3f, 0.3f, 0.8f, 1.f }, {} },
};
*/