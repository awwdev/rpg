//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData_General.hpp"

#include "res/Models/ModelLoader.hpp"
#include "res/Models/ModelPrimitives.hpp"
#include "res/Models/ModelType.hpp"
#include "res/Models/ModelView.hpp"
#include "res/Models/ModelMaps.hpp"

#include "com/box/Array.hpp"
#include "com/box/EnumMap.hpp"
#include "com/Range.hpp"
#include "dbg/Assert.hpp"

namespace rpg::res {

constexpr idx_t ALL_MODEL_VERTICES_MAX = 1'000'000;

struct Resources_Models
{
    com::Array<ModelVertex, ALL_MODEL_VERTICES_MAX> allVertices;
    ModelView modelViews [(idx_t) ModelType::ENUM_END];

    void Load()
    {
        allVertices.Clear();
        FOR_CARRAY(modelViews, i)
            modelViews[i] = {};

        idx_t modelViewIndex = 0;
        
        //? MODELS HARDCODED
        {
            constexpr idx_t ENUM_END = (idx_t) ModelType::HARDCODED_ENUM_END;
            const auto& map = MAP_MODELS_HARDCODED;

            dbg::Assert(map.usedIndices.Count() == ENUM_END, "mapping missing");
            for(idx_t i = 0; i < ENUM_END; ++i) {
                const auto& modelView = map.Get(i);
                const auto& meshView  = modelView.meshViews[0]; //assumes one mesh
                allVertices.AppendArray(meshView.vertPtr, meshView.vertCount);   
                modelViews[modelViewIndex] = modelView;
                ++modelViewIndex;
            }
        }

        //? MODELS LOADED externally
        {
            constexpr idx_t ENUM_END = (idx_t) ModelType::LOADED_ENUM_END;
            const auto& map = MAP_MODELS_LOADED;

            dbg::Assert(map.usedIndices.Count() == ENUM_END, "mapping missing");
            for(idx_t i = 0; i < ENUM_END; ++i) { //offset on map!
                chars_t path = map.Get(i).data;
                const auto modelView = LoadModel(allVertices, path); //appends
                modelViews[modelViewIndex] = modelView;
                ++modelViewIndex;
            }
        }

    }

};

} //ns