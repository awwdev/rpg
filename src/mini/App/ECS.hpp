//https://github.com/awwdev

#pragma once
#include "mini/Box/Bitset.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Math/Matrix.hpp"

namespace mini::app::ecs
{
    using ID = u32;
    constexpr ID ENTITY_COUNT_MAX = 1000;
    
    //? COMPONENTS

    struct C_Transform
    {
        math::Vec3f pos;
    };

    struct C_RenderData
    {

    };

    

    //? ECS

    struct ComponentArrays
    {
        template<class T>
        struct ComponentArray
        {
            box::Array<T, ENTITY_COUNT_MAX> dense;
            ID lookup [ENTITY_COUNT_MAX];
        };

        //? COMPONENT ARRAYS DEFINITION ---------------------------------------------
        ComponentArray<C_Transform>  transforms;
        ComponentArray<C_RenderData> renderData;

        template<class COMPONENT>
        auto& GetArray(){
            if constexpr(std::is_same_v<COMPONENT, C_Transform>)  return transforms;
            if constexpr(std::is_same_v<COMPONENT, C_RenderData>) return renderData;
        }
        //? COMPONENT ARRAYS DEFINITION ---------------------------------------------
    };

    struct ECS
    {
        box::Bitset<ENTITY_COUNT_MAX> entities;
        ComponentArrays arrays;
        
        ID AddEntity()
        {
            //TODO: using a remove table first
            const auto freeId = entities.FindFirstFreeBit();
            entities.Set<true>(freeId);
            return freeId;
        }

        template<class COMPONENT, class... CtorArgs>
        void AddComponent(const ID id, const CtorArgs&&... args)
        {
            auto& arr = arrays.GetArray<COMPONENT>();
            arr.dense.Append(args...); //forward?
            arr.lookup[id] = arr.dense.Count();
        }

    };



}//ns