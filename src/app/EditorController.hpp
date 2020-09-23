//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include "com/box/Matrix.hpp"
#include "wnd/WindowEvents.hpp"
#include "gpu/Meta/Cameras.hpp"
#include "app/InputMode.hpp"
#include "gpu/RenderData/RenderData.hpp"

namespace rpg::app {

struct EditorController
{
    gpu::EgoCamera camera;
    bool terrainWireMode = true;

    EditorController()
    {
        camera.Update(0);
    }

    void Update(const double dt, ecs::ECS&, gpu::RenderData& renderData)
    {
        if (app::glo::inputMode == app::glo::FlyMode)
            camera.Update(dt);
        if (wnd::glo::resizeState == wnd::glo::ResizeState::End)
            camera.UpdatePerspective();

        if (wnd::HasEvent<wnd::EventType::F9, wnd::EventState::Pressed>())
            terrainWireMode = !terrainWireMode;

        renderData.general.uboMeta.view = camera.view;
        renderData.general.uboMeta.proj = camera.perspective;
    }
};

}//ns