//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/Matrix.hpp"
#include "res/Mesh/MeshEnum.hpp"
#include "gpu/RenderData/MaterialSettings.hpp"

namespace rpg::gpu {

struct RenderData_Main
{
    //? meta --------------------------------------------------------------------
    static constexpr uint32_t TERRA_VERTICES_MAX_ALL = 1'000'000;
    static constexpr uint32_t TERRA_INDICES_MAX_ALL  = 1'000'000;
    static constexpr uint32_t MESH_INSTANCES_MAX = 100;

    struct Vertex
    {
        com::Vec3f pos;
        com::Vec3f nor;
        com::Vec4f col;
        com::Vec2f tex;
    };

    struct Meta
    {
        alignas(64) com::Mat4f view;
        alignas(64) com::Mat4f proj;
        alignas(16) com::Vec3f viewDir;
        alignas(16) com::Vec3f viewPos;
        alignas(16) float time;
        //global world offset
    };

    struct InstanceData
    {
        alignas(64) com::Mat4f transform;
        MaterialSettings materialSettings;
    };
 
    //? data --------------------------------------------------------------------
    Meta meta;
    com::Array<InstanceData, 100> instance_datas [(idx_t)res::MeshMaterialEnum::ENUM_END][(idx_t)res::MeshEnum::ENUM_END];
    bool enableTerrainWire = true;

    void Clear()
    {
        //meta is just partly overwritten not cleared
        for(auto i = 0; i < (idx_t)res::MeshMaterialEnum::ENUM_END; ++i) {
        for(auto j = 0; j < (idx_t)res::MeshEnum::ENUM_END; ++j) {
            instance_datas[i][j].Clear();
        }}
    }
};

}//ns