//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/Matrix.hpp"
#include "res/Mesh/MeshEnum.hpp"

namespace rpg::gpu {

struct RenderData_General
{
    static constexpr uint32_t TERRA_VERT_MAX_ALL = 1'000'000;
    static constexpr uint32_t MESH_INSTANCES_MAX =     1'000;

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
        com::Vec3f viewDir;
        float time;
    };

    struct MeshInstance
    {
        alignas(64) com::Mat4f transform;
        alignas(64) float metallic; 
        //should not be per instance! but per mesh, could use pushConst or ubo for material data 
    };
 
    //? DATA
    Meta meta;
    com::Array<MeshInstance, MESH_INSTANCES_MAX> meshInstances [(idx_t) res::MeshEnum::ENUM_END]; //big

    //set on gpu draw
    idx_t dbgVertCountInstanced = 0, dbgVertCountInstancedPrev = 0; 
    idx_t dbgVertCountTerrain = 0, dbgVertCountTerrainPrev = 0;

    void Clear()
    {
        //do not clear meta time

        FOR_C_ARRAY(meshInstances, i)
            meshInstances[i].Clear();  

        dbgVertCountInstancedPrev = dbgVertCountInstanced;
        dbgVertCountInstanced = 0;
        dbgVertCountTerrainPrev = dbgVertCountTerrain;
        dbgVertCountTerrain = 0;
    }
};

}//ns