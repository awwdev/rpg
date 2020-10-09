//https://github.com/awwdev

#include "gui/Widgets/Widget_Window.hpp"
#include "gui/Widgets/Widget_List.hpp"

#include "ecs/ECS.hpp"
#include <functional>

namespace rpg::gui {

struct GUI_Scene
{
    Widget_Window wnd 
    {
        .title = "Scene",
        .rect  = { 0, 0, 300, 500 }
    };

    Widget_List<ecs::ID> entityList
    {
        .listName = "Entities",
        .maxHeight = wnd.rect.height - wnd.BAR_H - 4,
    };
    ecs::ID selectedID = ecs::ECS_NULL;


    void Update(gpu::RenderData& renderData, ecs::ECS& ecs)
    {
        //TODO: subscription model instead of hard update per frame

        //? UPDATE SCENE TREE

        std::function<void(ecs::ID const, Widget_List<ecs::ID>::Item* const)> 
        addItem = [&](ecs::ID const entityID, Widget_List<ecs::ID>::Item* const prevParentItem)
        {
            //entity name
            com::String<100> entityName {};
            if (auto const* nameComponent = ecs.arrays.nameComponents.GetPtr(entityID)) 
            {
                entityName = nameComponent->name;
            }
            entityName.AppendArray(" #");
            entityName.AppendArithemtic(entityID);
            auto* currParentItem = entityList.AddItem(entityName.Data(), entityName.Length(), prevParentItem);

            //children
            if (auto const* mainComponent = ecs.arrays.mainComponents.GetPtr(entityID)) 
            {
                auto const& children = mainComponent->children;
                FOR_ARRAY(children, i){
                    auto const childID = children[i];
                    addItem(childID, currParentItem);
                }
            }
        };

        entityList.topLevelItems.Clear();
        FOR_ARRAY(ecs.entitiesTopLevel, i)
        {
            auto const entityID = ecs.entitiesTopLevel[i];
            addItem(entityID, nullptr);           
        }

        //? UPDATE
        wnd.Update(renderData);
        entityList.Update(renderData, wnd);
        
    }
};




}//ns