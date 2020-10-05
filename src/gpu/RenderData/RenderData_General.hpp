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
        alignas(16) com::Vec3f pos;
        alignas(16) com::Vec3f nor;
        alignas(16) com::Vec4f col;
        alignas(16) com::Vec2f tex;
    };

    struct Meta
    {
        alignas(64) com::Mat4f view;
        alignas(64) com::Mat4f proj;

        float time;
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
    com::Array<MeshInstance, MESH_INSTANCES_MAX> meshInstances [(idx_t) res::MeshEnum::ENUM_END]; //big

    //set on gpu draw
    idx_t dbgVertCountInstanced = 0, dbgVertCountInstancedPrev = 0; 
    idx_t dbgVertCountTerrain = 0, dbgVertCountTerrainPrev = 0;


    //TODO: additionally sorting by material 
    //TODO: also use one big array and not 2d array
    //rendersystem, renderdata_general, state_general

    void Clear()
    {
        //do not clear meta (and time)

        FOR_C_ARRAY(meshInstances, i)
            meshInstances[i].Clear();  

        dbgVertCountInstancedPrev = dbgVertCountInstanced;
        dbgVertCountInstanced = 0;

        dbgVertCountTerrainPrev = dbgVertCountTerrain;
        dbgVertCountTerrain = 0;
    }
};

}//ns