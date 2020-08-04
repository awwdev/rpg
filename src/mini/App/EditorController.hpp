//https://github.com/awwdev

#pragma once
#include "mini/ECS/ECS.hpp"
#include "mini/Math/Matrix.hpp"
#include "mini/window/WindowEvents.hpp"
#include "mini/Rendering/Cameras.hpp"
#include "mini/App/InputMode.hpp"

namespace mini::app {

struct EditorController
{
    rendering::EgoCamera camera;

    void Update(const double dt, ecs::ECS& ecs)
    {
        if (app::global::inputMode == app::global::FlyMode)
            camera.Update(dt);
    }
};

}//ns