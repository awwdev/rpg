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
        entityList.topLevelItems.Clear();
        idx_t currentIdx = 0;

        std::function<void(ecs::ID const, Widget_List<ecs::ID>::Item*)> addEntity = [&](ecs::ID const entityID, Widget_List<ecs::ID>::Item* parent)
        {
             if (auto* const nameComponent = ecs.arrays.nameComponents.GetOptional(entityID))
             {
                if (entityList.activeIdx == currentIdx)
                    selectedID = entityID;
                //! BUG: THIS GIVES NOT THE RIGHT E_IDX

                com::String<100> entityName = nameComponent->name;
                entityName.AppendArray(" #");
                entityName.AppendArithemtic(entityID);

                auto* currParent = entityList.AddItem(entityName.Data(), entityName.Length(), parent);
                currentIdx++;

                if (auto* const mainComponent = ecs.arrays.mainComponents.GetOptional(entityID)) {
                    FOR_ARRAY(mainComponent->children, i)
                        addEntity(mainComponent->children[i], currParent);
                }
            }
        };

        FOR_ARRAY(ecs.entitiesUsed, i){
            auto const entityID = ecs.entitiesUsed[i];
            addEntity(entityID, nullptr);
        }        

        dbg::LogInfo(selectedID);

        //? UPDATE
        wnd.Update(renderData);
        entityList.Update(renderData, wnd);
        
    }
};

}//ns