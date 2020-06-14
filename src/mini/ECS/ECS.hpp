//https://github.com/awwdev

#pragma once
#include "mini/Box/Bitset.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Math/Matrix.hpp"

#include "mini/ECS/EntityID.hpp"
#include "mini/ECS/ComponentArray.hpp"


namespace mini::ecs
{
    struct ECS
    {
        ComponentArrays<MAX_ENTITY_COUNT> arrays;
        

    };    





    /*

    //?ADD COMPONENTS--------------------------------

    enum ComponentType
    {
        Transform,
        ENUM_END
    };

    #define DEFINE_COMPONENT_ARRAYS() \
        ComponentArray<C_Transform> transforms;

    #define GET_COMPONENT_ARRAY_BY_TYPE() \
        if constexpr(std::is_same_v<COMPONENT, C_Transform>) return transforms; \
        else static_assert(0, "component type missing");

    #define GET_COMPONENT_ENUM_BY_TYPE() \
        if constexpr(std::is_same_v<COMPONENT, C_Transform>) return ComponentType::Transform; \
        else static_assert(0, "component enum missing");

    #define REMOVE_ALL_COMPONENTS() \
        if (signatures[entityID].Test(ComponentType::Transform)) RemoveComponent<C_Transform>(entityID);

    //?-----------------------------------------------


    template<class COMPONENT>
    struct ComponentArray
    {
        box::Array<COMPONENT, ENTITY_COUNT_MAX> dense;
        ID cLookup [ENTITY_COUNT_MAX] { NONE }; //lookup component via entity id
        ID eLookup [ENTITY_COUNT_MAX] { NONE }; //lookup entity via dense id 

        COMPONENT*       GetOptional(const ID entityID)         { return cLookup[entityID] == NONE ? nullptr : &dense[cLookup[entityID]]; }    
        const COMPONENT* GetOptional(const ID entityID) const   { return cLookup[entityID] == NONE ? nullptr : &dense[cLookup[entityID]]; }
        COMPONENT&       Get(const ID entityID)                 { return dense[cLookup[entityID]]; }    
        const COMPONENT& Get(const ID entityID) const           { return dense[cLookup[entityID]]; }

        ComponentArray() 
        { 
            //! dont init that way ... use 0 as NONE instead
            FOR_CARRAY(cLookup, i) cLookup[i] = NONE;
            FOR_CARRAY(eLookup, i) eLookup[i] = NONE;
        }
    };

    template<class COMPONENT>
    constexpr ComponentType GetComponentType() {
        GET_COMPONENT_ENUM_BY_TYPE()
    }

    struct ComponentArrays
    {
        DEFINE_COMPONENT_ARRAYS() 

        template<class COMPONENT>
        auto& GetArray(){
            GET_COMPONENT_ARRAY_BY_TYPE()
        }
    };

    struct ECS
    {
        box::Bitset<ENTITY_COUNT_MAX> entities;
        ID entityCount = 0;
        box::Array<ID, 10> removedEntities;
        box::Bitset<ComponentType::ENUM_END> signatures [ENTITY_COUNT_MAX];
        ComponentArrays arrays;

        inline auto AddEntity()
        {
            const ID freeId = (removedEntities.Empty()) 
                ? entities.FindFirstFreeBit() 
                : removedEntities.RemoveLast();
            entities.Set<true>(freeId);
            ++entityCount;
            return freeId;
        }

        inline void RemoveEntity(const ID entityID)
        {
            if (entities.Test(entityID) == false) {
                WARN("removing entity that already has been removed");
                return;
            }
            removedEntities.Append(entityID); //!currently no check for pool exhaustion
            entities.Set<false>(entityID);
            --entityCount;
            
            REMOVE_ALL_COMPONENTS()
        }

        template<class COMPONENT, class... CtorArgs>
        void AddComponent(const ID entityID, CtorArgs&&... args)
        {
            auto& arr = arrays.GetArray<COMPONENT>();

            const auto* const component = arr.GetOptional(entityID);
            if (component != nullptr){
                WARN("adding component that already exists");
                return;
            }

            arr.dense.Append(std::forward<CtorArgs>(args)...);
            const auto lastDenseIdx = arr.dense.Count() - 1;
            arr.cLookup[entityID] = lastDenseIdx;
            arr.eLookup[lastDenseIdx] = entityID;
            signatures[entityID].Set<true>(GetComponentType<COMPONENT>());
        }

        template<class COMPONENT>
        void RemoveComponent(const ID id)
        {
            auto& arr = arrays.GetArray<COMPONENT>();

            const auto* const component = arr.GetOptional(id);
            if (component == nullptr){
                WARN("removing component that does not exist");
                return;
            }

            const auto denseId = arr.cLookup[id];
            arr.dense.RemoveSwap(denseId);
            arr.cLookup[id] = NONE;
            signatures[id].Set<false>(GetComponentType<COMPONENT>());
            //fix swap
            arr.cLookup[arr.eLookup[arr.dense.Count()]] = denseId;
            arr.eLookup[denseId] = arr.eLookup[arr.dense.Count()]; //swap
            arr.eLookup[arr.dense.Count()] = NONE;
        }

    };

    */

}//ns