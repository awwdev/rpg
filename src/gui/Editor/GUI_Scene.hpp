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

        auto tmpEntities = ecs.entities;
        entityList.topLevelItems.Clear();

        std::function<void(ecs::ID const, Widget_List<ecs::ID>::Item*)> addItem = 
        [&](ecs::ID const id, Widget_List<ecs::ID>::Item* parent) 
        {
            if (auto const* nameComponent = ecs.arrays.nameComponents.GetOptional(id)){
                com::String<100> entityName = nameComponent->name;
                entityName.AppendArray(" #");
                entityName.AppendArithemtic(id);
                parent = entityList.AddItem(entityName.Data(), entityName.Length(), parent);
            }
            else return;

            if (auto const* mainComponent = ecs.arrays.mainComponents.GetOptional(id)){
                FOR_ARRAY(mainComponent->children, i){
                    auto const childIdx = mainComponent->children[i];
                    addItem(childIdx, parent);
                    tmpEntities.Set(childIdx, false); //! does not work, also in general not great (because of shuffle)
                }
            }
        };      

        FOR_BITSET(tmpEntities, idx) {
            if (tmpEntities.Test(idx) == false)
                continue;
            addItem(idx, nullptr);
        }

        //? UPDATE
        wnd.Update(renderData);
        entityList.Update(renderData, wnd);
        
    }
};




}//ns