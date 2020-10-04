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
    com::Vec3f scale        { 1, 1, 1 }; 
    com::Vec3f rotation     {};
    com::Vec3f translation  {};
    com::Mat4f transform    {};

    //render data
    res::MeshEnum         meshEnum          { res::MeshEnum::None };
    res::MeshMaterialEnum meshMaterialEnum  { res::MeshMaterialEnum::Default };
    bool indexed { false };

    //parsing
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
                    const auto values = ValStrToValArray<3, 100>(val_cstr);     
                    dbg::Assert(!values.Empty(), "values are empty");
                    FOR_ARRAY(values, i){
                        auto prefabEnum = res::PREFAB_STR_TO_ENUM.GetOptional(values[i].cstr);
                        dbg::Assert(prefabEnum, "child prefab enum wrong");
                        children.Append((ID) *prefabEnum);
                    }
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

                case ComponentDataEnum::Indexed: 
                {
                    auto const val = std::atoi(val_cstr);
                    indexed = static_cast<bool>(val);
                } 
                break;

                default: dbg::Assert(false, "wrong component data enum");
            };

        }
    }

};

}//ns