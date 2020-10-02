//https://github.com/awwdev

#pragma once
#include "ecs/ComponentsMeta/ComponentData.hpp"
#include "res/Mesh/MeshEnum.hpp"
#include "res/Mesh/MeshMaterialEnum.hpp"
#include "com/box/Array.hpp"
#include "com/Matrix.hpp"

namespace rpg::ecs {

struct MainComponent
{
    static constexpr idx_t CHILDREN_MAX = 10;

    //nesting data
    com::Array<ID, CHILDREN_MAX> children;

    //transform data
    com::Vec3f scale; 
    com::Vec3f rotation; 
    com::Vec3f translation; 

    //render data
    res::MeshEnum         meshEnum;
    res::MeshMaterialEnum meshMaterialEnum;

    MainComponent(ecs::ComponentDataPairs const& pairs)
    {

        FOR_ARRAY(pairs, i) {
            auto const& key_cstr = pairs[i].key.cstr;
            auto const& val_cstr = pairs[i].val.cstr;

            auto componentDataEnum = ecs::COMPONENT_DATA_STR_TO_ENUM.GetOptional(key_cstr);
            dbg::Assert(componentDataEnum, "no componentDataEnum");

            switch(*componentDataEnum)
            {
                case ComponentDataEnum::Children:  
                {

                }
                break;

                case ComponentDataEnum::Mesh:      
                {
                    auto meshEnum = res::MESH_STR_TO_ENUM.GetOptional(val_cstr);
                    dbg::Assert(meshEnum, "no meshEnum");
                    this->meshEnum = *meshEnum;
                }
                break;

                case ComponentDataEnum::Material: 
                {
                    auto meshMaterialEnum = res::MESH_MATERIAL_STR_TO_ENUM.GetOptional(val_cstr);
                    dbg::Assert(meshMaterialEnum, "no meshEnum");
                    this->meshMaterialEnum = *meshMaterialEnum;
                } 
                break;

                default: dbg::Assert(false, "wrong component data enum");
            };

        }
    }
};

}//NS
