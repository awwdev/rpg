//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData_General.hpp"

#include "res/Model/Model.hpp"
#include "res/Model/ModelPrimitives.hpp"
#include "res/Model/ModelType.hpp"
#include "res/Model/ModelMaps.hpp"

#include "com/box/Array.hpp"
#include "com/box/EnumMap.hpp"
#include "com/Range.hpp"
#include "dbg/Assert.hpp"

/*
namespace rpg::res {

struct Resources_Models
{
    using RD = gpu::RenderData_General;

    Model models [(idx_t) ModelType::ENUM_END]; //allVertices layout info
    com::Array<ModelVertex, RD::MODEL_VERT_MAX_ALL> allVertices;

    void Load()
    {
        allVertices.Clear();
        FOR_C_ARRAY(models, i)
            models[i] = {};

        //? MODELS HARDCODED
        {
            idx_t constexpr enumEnd = (idx_t) ModelType::HARDCODED_ENUM_END;
            auto const& map = MAP_MODELS_HARDCODED;
            dbg::Assert(map.usedIndices.Count() == enumEnd, "mapping missing");

            for(idx_t modelTypeIdx = 0; modelTypeIdx < enumEnd; ++modelTypeIdx) {
                auto const& model = map.Get(modelTypeIdx);
                models[modelTypeIdx] = model;

                FOR_ARRAY(model.meshes, meshIdx){
                    const auto vertCountPrev = allVertices.Count();
                    const auto& mesh = model.meshes[meshIdx];
                    allVertices.AppendArray(mesh.vertPtr, mesh.vertCount);
                    //point to the vertex array and not the const storage
                    models[modelTypeIdx].meshes[meshIdx].vertPtr = &allVertices[vertCountPrev];
                }
            }
        }

        //? MODELS LOADED
        {
            idx_t constexpr enumEnd = (idx_t) ModelType::LOADED_ENUM_END;
            auto const& map = MAP_MODELS_LOADED;
            dbg::Assert(map.usedIndices.Count() == enumEnd, "mapping missing");

            for(idx_t modelTypeIdx = 0; modelTypeIdx < enumEnd; ++modelTypeIdx) { //OFFSET!
                auto const& path = map.Get(modelTypeIdx);
                const auto model = LoadModel(allVertices, path.data); //loads all meshes
                const auto modelTypeIdxOffsetted = modelTypeIdx + (idx_t) ModelType::LOADED_ENUM_BEGIN;
                models[modelTypeIdxOffsetted] = model;
            }
        }
        
    }

};

} //ns

*/