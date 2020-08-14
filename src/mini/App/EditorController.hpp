//https://github.com/awwdev

#pragma once
#include "mini/ECS/ECS.hpp"
#include "mini/Utils/Matrix.hpp"
#include "mini/window/WindowEvents.hpp"
#include "mini/Rendering/Cameras.hpp"
#include "mini/App/InputMode.hpp"

namespace mini::app {

struct EditorController
{
    rendering::EgoCamera camera;
    bool terrainWireMode = true;

    EditorController()
    {
        camera.Update(0);
    }

    void Update(const double dt, ecs::ECS& ecs)
    {
        if (app::global::inputMode == app::global::FlyMode)
            camera.Update(dt);

        if (wnd::HasEvent<wnd::F9, wnd::Pressed>())
            terrainWireMode = !terrainWireMode;

    }
};

}//ns