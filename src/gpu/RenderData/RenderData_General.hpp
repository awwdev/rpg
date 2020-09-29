//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/Matrix.hpp"
#include "res/Models/ModelType.hpp"

namespace rpg::gpu {

struct RenderData_General
{
    static constexpr uint32_t TERRA_VERT_MAX_ALL = 1'000'000;
    static constexpr uint32_t MODEL_VERT_MAX_ALL = 1'000'000;
    static constexpr uint32_t MODEL_INST_MAX     =     1'000;

    struct Vertex
    {
        alignas(16) com::Vec3f pos;
        alignas(16) com::Vec3f nor;
        alignas(16) com::Vec4f col;
        alignas(16) com::Vec2f tex;
    };

    struct Meta
    {
        alignas(64) com::Mat4f view;
        alignas(64) com::Mat4f proj;
    };

    struct ModelInstance
    {
        com::Mat4f transform;
        //material settings
    };

    struct MeshStats
    {
        idx_t instanceCount;
    };

    //? DATA
    Meta meta;
    com::Array<ModelInstance, MODEL_INST_MAX> modelInstances;
    MeshStats meshStats [(idx_t) res::ModelType::ENUM_END];

    void Clear()
    {
        meta = {};
        modelInstances.Clear();
        FOR_CARRAY(meshStats, modelTypeIdx)
            meshStats[modelTypeIdx] = {};

    }
};

}//ns