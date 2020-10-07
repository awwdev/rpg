//https://github.com/awwdev

#include "gui/Widgets/Widget_Window.hpp"
#include "gui/Widgets/Widget_Table.hpp"
#include "com/DeltaTime.hpp"
#include "com/box/EnumMap.hpp"
#include "com/box/String.hpp"
#include "dbg/Assert.hpp"

namespace rpg::gui {

struct GUI_Stats
{
    //table rows
    enum class RowEnum 
    {
        fps,
        dt,
        shadowMapSize,
        shadowCascades,
        instancedVerts,
        terrainVerts,
        ENUM_END
    };
    const com::EnumMap<RowEnum::ENUM_END, com::String<100>> RowStrings
    {
        { RowEnum::fps,                 "fps" },
        { RowEnum::dt,                  "dt"  },
        { RowEnum::shadowMapSize,       "shadowMapSize"  },
        { RowEnum::shadowCascades,      "shadowCascades"  },
        { RowEnum::instancedVerts,      "instancedVerts"  },
        { RowEnum::terrainVerts,        "terrainVerts"  },
    };    

    //widgets
    Widget_Window wnd 
    {
        .title  = "Stats",
        .rect   = { (f32)wnd::glo::window_w - 300, 0, 300, 200 },
        .limits = { 128, 128, f32max, f32max }
    };
    Widget_Table table;


    GUI_Stats()
    {
        for(idx_t i = 0; i < (idx_t) RowEnum::ENUM_END; ++i)
        {
            dbg::Assert(RowStrings.Contains(i), "enum map entry missing");
            const auto str = RowStrings.Get(i);
            auto& row = table.table.Append();
            row.Append(str);
            row.Append("0");
        }
    }

    void Update(gpu::RenderData& renderData)
    {
        wnd.Update(renderData);

        if (dt::secondHasPassed)
        {
            table.table[(idx_t) RowEnum::fps][1] = dt::fps;
            table.table[(idx_t) RowEnum::dt ][1] = dt::seconds;

            table.table[(idx_t) RowEnum::shadowCascades][1] = renderData.shadow.CASCADE_COUNT;
            table.table[(idx_t) RowEnum::shadowMapSize][1]  = renderData.shadow.SHADOW_MAP_SIZE;

            table.table[(idx_t) RowEnum::instancedVerts][1] = renderData.general.dbgVertCountInstancedPrev;
            table.table[(idx_t) RowEnum::terrainVerts][1] = renderData.general.dbgVertCountTerrainPrev;

        }
        
        table.Update(renderData, wnd);
    }
};

}//ns