//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources.hpp"
#include "mini/App/Scene.hpp"
#include "mini/Resources/HostResources.hpp"

namespace mini::vk {

void UpdateVkResources_GameScene(VkResources& resources, const app::GameScene& scene, res::HostResources& hostRes, double dt)
{
    const utils::Mat4f BIAS { 
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.5, 0.5, 0.0, 1.0 
    };

    //resources.common_pushConsts.camera = scene.camera.GetOrthographic() * scene.sun.GetView();
    //resources.common_pushConsts.camera = scene.camera.GetPerspective()  * scene.playerController.GetView(scene.ecs);
    //resources.common_pushConsts.camera = scene.camera.GetPerspective()  * scene.camera.GetView();

    //TODO: this should be handled somewhere else which is active
    if (app::global::inputMode == app::global::PlayMode)
        resources.common_pushConsts.camera = resources.sky.pushConsts.camera = scene.playerController.camera.perspective * scene.playerController.camera.view;
    else
        resources.common_pushConsts.camera = resources.sky.pushConsts.camera = scene.editorController.camera.perspective * scene.editorController.camera.view;
        //resources.common_pushConsts.camera = scene.sun.GetOrthographic() * scene.sun.GetView();

    static float t = 0;
    t+=(f32)dt;
    resources.common_pushConsts.time = t;

    resources.common_pushConsts.sun    = scene.sun.GetOrthographic() * scene.sun.GetView(); //BIAS * 
    resources.common_pushConsts.sunDir = utils::Normalize(scene.sun.pos * 1);
    resources.ui.pushConsts.wnd_w = wnd::global::window_w;
    resources.ui.pushConsts.wnd_h = wnd::global::window_h;
    resources.sky.pushConsts.topColor = { 0.1f, 0.1f, 1.0f, 1 };
    resources.sky.pushConsts.botColor = { 1.0f, 1.0f, 1.0f, 1 };

    resources.ui.ubo.Clear();
    resources.ui.ubo.Store(scene.renderGraph.ui_ubo);

    resources.default.ubo.Clear();
    resources.default.ubo.Store(scene.renderGraph.default_ubo);

    //update terrain quadrant that is edited
    FOR_ARRAY(hostRes.terrain.settings.dirtyQuadrants, i){
        const auto quadrantIdx = hostRes.terrain.settings.dirtyQuadrants[i];
        resources.terrain.vbo.UpdateGroup(quadrantIdx, hostRes.terrain.GetQuadrant(quadrantIdx).verts);
    }
   
}

} //ns