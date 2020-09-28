//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData_General.hpp"

#include "res/Models/Model.hpp"
#include "res/Models/ModelLoader.hpp"
#include "res/Models/ModelPrimitives.hpp"
#include "res/Models/ModelType.hpp"
#include "res/Models/ModelMaps.hpp"

#include "com/box/Array.hpp"
#include "com/box/EnumMap.hpp"
#include "com/Range.hpp"
#include "dbg/Assert.hpp"

namespace rpg::res {

struct Resources_Models
{
    using RD = gpu::RenderData_General;

    Model models [(idx_t) ModelType::ENUM_END];
    com::Array<ModelVertex, RD::MODEL_VERT_MAX_ALL> allVertices;

    void Load()
    {
        allVertices.Clear();
        FOR_CARRAY(models, i)
            models[i] = {};

        //? MODELS HARDCODED
        {
            idx_t constexpr enumEnd = (idx_t) ModelType::HARDCODED_ENUM_END;
            auto const& map = MAP_MODELS_HARDCODED;
            dbg::Assert(map.usedIndices.Count() == enumEnd, "mapping missing");

            for(idx_t modelType = 0; modelType < enumEnd; ++modelType) {
                auto const& model = map.Get(modelType);
                models[modelType] = model;

                FOR_ARRAY(model.meshes, meshIdx){
                    const auto vertCountPrev = allVertices.Count();
                    const auto& mesh = model.meshes[meshIdx];
                    allVertices.AppendArray(mesh.vertPtr, mesh.vertCount);   
                    //point to the big vertex array instead of const storage
                    models[modelType].meshes[meshIdx].vertPtr = &allVertices[vertCountPrev];
                }
            }
        }

        //? MODELS LOADED
        {
            idx_t constexpr enumEnd = (idx_t) ModelType::LOADED_ENUM_END;
            auto const& map = MAP_MODELS_LOADED;
            dbg::Assert(map.usedIndices.Count() == enumEnd, "mapping missing");

            for(idx_t modelType = 0; modelType < enumEnd; ++modelType) { //OFFSET!
                auto const& path = map.Get(modelType);
                const auto model = LoadModel(allVertices, path.data);
                idx_t const modelTypeOffset = modelType + (idx_t) ModelType::LOADED_ENUM_BEGIN;
                models[modelTypeOffset] = model;
            }
        }
        
    }

};

} //ns