//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/Matrix.hpp"
#include "res/Mesh/MeshEnum.hpp"

namespace rpg::gpu {

struct RenderData_General
{
    static constexpr uint32_t TERRA_VERTICES_MAX_ALL = 1'000'000;
    static constexpr uint32_t TERRA_INDICES_MAX_ALL  = 1'000'000;
    static constexpr uint32_t MESH_INSTANCES_MAX = 1'000;

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
        alignas(16) float time;
        //TODO: global world offset
    };

    struct MeshInstance
    {
        alignas(64) com::Mat4f transform;
        alignas(64) struct 
        {
            float metallic;
            float glow;
        } 
        renderSettings;
    };
 
    //? DATA
    Meta meta;
    com::Array<MeshInstance, MESH_INSTANCES_MAX> meshInstances [(idx_t) res::MeshEnum::ENUM_END]; //big
    bool enableTerrainWire = true;

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

std::ostream& operator<<(std::ostream& os, RenderData_General::Vertex const& vertex)
{
    PrintMatrix(vertex.pos);
    PrintMatrix(vertex.nor);
    PrintMatrix(vertex.col);
    PrintMatrix(vertex.tex);
    return os;
}

}//ns