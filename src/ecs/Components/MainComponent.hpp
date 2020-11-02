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
    com::Mat4f transform    { com::Identity4() };

    //render data
    res::MeshEnum  meshEnum { res::MeshEnum::None };
    float metallic { 0.f };
    float glow { 0.f };
    float flat { 0.f };
    com::Vec4f baseColor { 1, 0, 0, 1 }; //!bad, dumping place

    //parsing
    MainComponent() = default;
    MainComponent(ecs::ComponentDataPairs const& pairs)
    {
        FOR_ARRAY(pairs, i) {
            auto const& key_cstr = pairs[i].key.Data();
            auto const& val_cstr = pairs[i].val.Data();

            auto componentDataEnum = ecs::COMPONENT_DATA_STR_TO_ENUM.GetOptional(key_cstr);
            dbg::Assert(componentDataEnum, "no componentDataEnum");

            switch(*componentDataEnum)
            {
                case ComponentDataEnum::Children:  
                {
                    const auto values = ValStrToValArray<3, 100>(val_cstr);     
                    dbg::Assert(!values.Empty(), "values are empty");
                    FOR_ARRAY(values, i){
                        auto prefabEnum = res::PREFAB_STR_TO_ENUM.GetOptional(values[i].Data());
                        dbg::Assert(prefabEnum, "child prefab enum wrong");
                        children.AppendElement((ID) *prefabEnum);
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

                case ComponentDataEnum::Scale:      
                {
                    const auto values = ValStrToValArray<3, 10>(val_cstr);   
                    dbg::Assert(!values.Empty(), "values are empty");
                    FOR_ARRAY(values, i){
                        scale[0][i] = std::atof(values[i].Data());
                    }
                }
                break;

                case ComponentDataEnum::Metallic:      
                {
                    metallic = std::atof(val_cstr);
                }
                break;

                case ComponentDataEnum::Glow:      
                {
                    glow = std::atof(val_cstr);
                }
                break;

                case ComponentDataEnum::Flat:      
                {
                    flat = std::atof(val_cstr);
                }
                break;

                default: dbg::Assert(false, "wrong component data enum");
            };

        }
    }

};

}//ns