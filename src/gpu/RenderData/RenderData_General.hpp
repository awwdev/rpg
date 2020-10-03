//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/Matrix.hpp"
#include "res/Mesh/MeshEnum.hpp"

namespace rpg::gpu {

struct RenderData_General
{
    static constexpr uint32_t TERRA_VERT_MAX_ALL = 1'000'000;
    static constexpr uint32_t MODEL_VERT_MAX_ALL = 1'000'000;
    static constexpr uint32_t MESH_INSTANCES_MAX =     1'000;
    static constexpr uint32_t MESH_MAX_ALL       =       100;

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

    struct MeshInstance
    {
        com::Mat4f transform;
    };

    struct MeshInstancesStats
    {
        idx_t instanceCount;
    };

    //? DATA
    Meta meta;
    com::Array<MeshInstance, MESH_INSTANCES_MAX> meshInstances [(idx_t) res::MeshEnum::ENUM_END];

    void Clear()
    {
        meta = {};
        FOR_C_ARRAY(meshInstances, i)
            meshInstances[i].Clear();  
    }
};

}//ns