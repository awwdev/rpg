//https://github.com/awwdev

#pragma once
#include "ECS/ECS.hpp"
#include "Utils/Matrix.hpp"
#include "window/WindowEvents.hpp"
#include "Rendering/Cameras.hpp"
#include "App/InputMode.hpp"

namespace rpg::app {

struct EditorController
{
    rendering::EgoCamera camera;
    bool terrainWireMode = true;

    EditorController()
    {
        camera.Update(0);
    }

    void Update(const double dt, ecs::ECS&)
    {
        if (app::global::inputMode == app::global::FlyMode)
            camera.Update(dt);
        if (wnd::global::resizeState == wnd::global::ResizeState::End)
            camera.UpdatePerspective();

        if (wnd::HasEvent<wnd::F9, wnd::Pressed>())
            terrainWireMode = !terrainWireMode;

    }
};

}//ns