//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/Matrix.hpp"
#include "res/Models/ModelType.hpp"

namespace rpg::gpu {

struct RenderData_General
{
    static constexpr uint32_t VBO_TERRAIN_MAX = 10'000;
    static constexpr uint32_t VBO_OBJECTS_MAX = 10'000;
    static constexpr uint32_t UBO_MODEL_MAX   =  1'000;

    struct Vertex
    {
        alignas(16) com::Vec3f pos;
        alignas(16) com::Vec3f nor;
        alignas(16) com::Vec4f col;
        alignas(16) com::Vec2f tex;
    };

    struct UBO_Meta
    {
        alignas(64) com::Mat4f view;
        alignas(64) com::Mat4f proj;
    };

    struct SBO_Model
    {
        com::Mat4f transform;
    };

    //? DATA
    UBO_Meta uboMeta;

    struct VertexRange { idx_t beginIdx, count; };
    VertexRange vertexRanges [(idx_t) res::ModelType::ENUM_END];
    com::Array<SBO_Model, UBO_MODEL_MAX> sboModels [(idx_t) res::ModelType::ENUM_END];

    //TODO: where to set the vertexRanges (on resource load?)
    //TODO: enum is cluttered due to how loading is done

    void Clear()
    {
        //uboMeta is overwritten
        FOR_CARRAY(sboModels, i)
            sboModels[i].Clear();
    }
};

}//ns