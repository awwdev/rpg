//https://github.com/awwdev

#include "gui/Widgets/Widget_Window.hpp"
#include "gui/Widgets/Widget_List.hpp"

#include "ecs/ECS.hpp"

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
        .listName = "Entities"

    };

    void Update(gpu::RenderData& renderData, ecs::ECS& ecs)
    {
        entityList.topLevelItems.Clear();
        FOR_ARRAY(ecs.entitiesUsed, i) {
            auto const  entityID = ecs.entitiesUsed[i];
            if (auto const nameComponent = ecs.arrays.nameComponents.GetOptional(entityID)){
                auto* parent = entityList.AddItem(nameComponent->name.Data(), nameComponent->name.Length());
            }
        }

        wnd.Update(renderData);
        entityList.Update(renderData, wnd);
    }
};

}//ns