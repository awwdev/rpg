//https://github.com/awwdev

#pragma once
#include "ECS/ECS.hpp"
#include "Common/Matrix.hpp"
#include "window/WindowEvents.hpp"
#include "GPU/Meta/Cameras.hpp"
#include "App/InputMode.hpp"
#include "GPU/RenderData/RenderData.hpp"

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

        renderData.general.uboData_general_meta.view = camera.view;
        renderData.general.uboData_general_meta.proj = camera.perspective;
    }
};

}//ns