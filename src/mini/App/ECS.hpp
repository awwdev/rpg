//https://github.com/awwdev

#pragma once
#include "mini/Box/Bitset.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Math/Matrix.hpp"

namespace mini::app::ecs
{
    //!65536
    //! 0 means non-existent component
    using ID = u16; 
    constexpr ID NONE = 9;//std::numeric_limits<ID>::max();
    constexpr ID ENTITY_COUNT_MAX = 10;

    enum ComponentType
    {
        Transform,
        ENUM_END
    };

    //TODO: move into own header
    struct C_Transform
    {
        math::Vec3f pos;
        //rotation
        //scale
    };

    template<class COMPONENT>
    struct ComponentArray
    {
        box::Array<COMPONENT, ENTITY_COUNT_MAX> dense;
        ID lookup  [ENTITY_COUNT_MAX] { 0 };
        ID reverse [ENTITY_COUNT_MAX] { 0 }; //get entity from component (reverse lookup)

        COMPONENT*       GetComponent(const ID entityID)        { return lookup[entityID] == NONE ? nullptr : &dense[lookup[entityID]]; }    
        const COMPONENT* GetComponent(const ID entityID) const  { return lookup[entityID] == NONE ? nullptr : &dense[lookup[entityID]]; }
    };

    struct ComponentArrays
    {
        ComponentArray<C_Transform> transforms;
        //...

        template<class COMPONENT>
        auto& GetArray(){
            if constexpr(std::is_same_v<COMPONENT, C_Transform>) return transforms;
            //...
            else static_assert(0, "component type missing");
        }

        template<class COMPONENT>
        constexpr ComponentType GetComponentType()
        {
            if constexpr(std::is_same_v<COMPONENT, C_Transform>) return ComponentType::Transform;
            //...
            else static_assert(0, "component idx missing");
        }
    };

    


    struct ECS
    {
        box::Bitset<ENTITY_COUNT_MAX> entities;
        box::Array<ID, 10> removedEntities;
        ComponentArrays arrays;
        box::Bitset<ComponentType::ENUM_END> signatures [ENTITY_COUNT_MAX];


        inline auto AddEntity()
        {
            const ID freeId = (removedEntities.Empty()) 
                ? entities.FindFirstFreeBit() 
                : removedEntities.RemoveLast();
            entities.Set<true>(freeId);
            return freeId;
        }

        inline void RemoveEntity(const ID id)
        {
            if (entities.Test(id) == false) {
                WARN("removing entity that already has been removed");
                return;
            }
            removedEntities.Append(id); //!currently no check for pool exhaustion
            entities.Set<false>(id);
            //TODO: remove all components
        }

        template<class COMPONENT, class... CtorArgs>
        void AddComponent(const ID id, CtorArgs&&... args)
        {
            auto& arr = arrays.GetArray<COMPONENT>();
            arr.dense.Append(std::forward<CtorArgs>(args)...);
            const auto lastDense = arr.dense.Count() - 1;
            arr.lookup[id] = lastDense;
            arr.reverse[lastDense] = id;
            signatures[id].Set<true>(arrays.GetComponentType<COMPONENT>());
        }

        template<class COMPONENT>
        void RemoveComponent(const ID id)
        {
            auto& arr = arrays.GetArray<COMPONENT>();
            if (auto* component = arr.GetComponent(id); component != nullptr){
                const auto denseId = arr.lookup[id];
                arr.dense.RemoveFast(denseId);
                arr.lookup[id] = NONE;
                signatures[id].Set<false>(arrays.GetComponentType<COMPONENT>());
                //fix swap
                const auto swappedId  = arr.reverse[denseId];
                arr.lookup[swappedId] = denseId;
                arr.reverse[denseId]  = swappedId;
            }
        }
        

    };

    




















    /*
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

            T&       GetComponent(const ID entityID)        { return dense[lookup[entityID]]; }
            const T& GetComponent(const ID entityID) const  { return dense[lookup[entityID]]; }
        };

        //? COMPONENT ARRAYS DEFINITION ---------------------------------------------
        enum class ComponentType //could be useful for prefabs too
        {
            Transform,
            RenderData,
            ENUM_END
        };

        box::Bitset<ComponentType::ENUM_END> signatures [ENTITY_COUNT_MAX];

        ComponentArray<C_Transform>  transforms;
        ComponentArray<C_RenderData> renderData;

        template<class COMPONENT>
        auto& GetArray(){
            if constexpr(std::is_same_v<COMPONENT, C_Transform>)  return transforms;
            if constexpr(std::is_same_v<COMPONENT, C_RenderData>) return renderData;
        }

        //? COMPONENT ARRAYS DEFINITION ---------------------------------------------

        template<class COMPONENT>
        void RemoveComponent(const ID entityID)
        {
            auto& arr = GetArray<COMPONENT>();
            const auto cid = arr.lookup[entityID];
            arr.dense.RemoveFast(cid); 
            
            if constexpr(std::is_same_v<COMPONENT, C_Transform>)  
                signatures[entityID].Set<false>(0);
            if constexpr(std::is_same_v<COMPONENT, C_RenderData>) 
                signatures[entityID].Set<false>(1);
            //TODO: update lookup from elsewhere
        }

        inline void RemoveAll(const ID entityID)
        {
            FOR_CARRAY(signatures, i) {
                if (signatures[i].Test(entityID)){
                    switch(i){
                        case 0: RemoveComponent<C_Transform>(entityID);  break;
                        case 1: RemoveComponent<C_RenderData>(entityID); break;
                    }
                }  
            }
        }
    };

    struct ECS
    {
        box::Bitset<ENTITY_COUNT_MAX> entities;
        ComponentArrays arrays;
        
        inline ID AddEntity()
        {
            //TODO: using a remove table first
            const auto freeId = entities.FindFirstFreeBit();
            entities.Set<true>(freeId);
            return freeId;
        }

        inline void RemoveEntity(const ID id)
        {
            entities.Set<false>(id);
            arrays.RemoveAll(id);
        }

        template<class COMPONENT, class... CtorArgs>
        void AddComponent(const ID entityID, const CtorArgs&&... args)
        {
            auto& arr = arrays.GetArray<COMPONENT>();
            arr.dense.Append(std::forward<CtorArgs>(args)...); //forward?
            arr.lookup[entityID] = arr.dense.Count() - 1;

            if constexpr(std::is_same_v<COMPONENT, C_Transform>)  
                arrays.signatures[entityID].Set<true>(0);
            if constexpr(std::is_same_v<COMPONENT, C_RenderData>) 
                arrays.signatures[entityID].Set<true>(1);
        }

        //TODO: deferred and immediate
        template<class COMPONENT>
        void RemoveComponentImmediate(const ID entityID)
        {
            auto& arr = arrays.GetArray<COMPONENT>();
            const auto cid = arr.lookup(entityID);
            arr.dense.RemoveFast(cid);
        }   

    };
    */



}//ns