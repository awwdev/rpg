//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/Matrix.hpp"
#include "res/Models/ModelType.hpp"

namespace rpg::gpu {

struct RenderData_General
{
    static constexpr uint32_t VBO_TERRAIN_MAX = 10'000;
    static constexpr uint32_t MODEL_VERT_COUNT_MAX = 10'000;
    static constexpr uint32_t MODEL_DATA_COUNT_MAX =  1'000;
    //static constexpr uint32_t MODEL_VERT_RANGES_MAX = 

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

    struct ModelData
    {
        com::Mat4f transform;
    };

    struct ModelTypeData 
    {
        idx_t instanceCount;
    };

    //? DATA
    Meta meta;

    com::Array<ModelData, MODEL_DATA_COUNT_MAX> modelData;
    ModelTypeData modelTypeData [(idx_t) res::ModelType::ENUM_END];

    void Clear()
    {
        //uboMeta is overwritten
        modelData.Clear();
        FOR_CARRAY(modelTypeData, i)
            modelTypeData[i] = {};
    }
};

}//ns