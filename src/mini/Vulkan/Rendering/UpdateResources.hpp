//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources/Resources.hpp"
#include "mini/Vulkan/Commands.hpp"
#include "mini/App/Scene.hpp"
#include "mini/Resources/HostResources.hpp"

namespace mini::vk {

void UpdateVkResources_GameScene(VkResources& resources, const app::GameScene& scene, res::HostResources& hostRes, double dt, Commands& commands)
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
    if (app::global::inputMode == app::global::PlayMode){
        resources.common_pushConsts.camera = resources.sky.pushConsts.camera = scene.playerController.camera.perspective * scene.playerController.camera.view;

        resources.common_pushConsts2.projection = scene.playerController.camera.perspective;
        resources.common_pushConsts2.view       = scene.playerController.camera.view;
    }
        
    else{
        resources.common_pushConsts.camera = resources.sky.pushConsts.camera = scene.editorController.camera.perspective * scene.editorController.camera.view;
        //resources.common_pushConsts.camera = scene.sun.GetOrthographic() * scene.sun.GetView();

        resources.common_pushConsts2.projection = scene.editorController.camera.perspective;
        resources.common_pushConsts2.view       = scene.editorController.camera.view;
    }

    //TODO: THIS IS THE ISSUE !!! we would need the right sun persp
    resources.common_pushConsts2.sunView = scene.sun.GetView();
    resources.common_pushConsts2.sunDir  = utils::Normalize(scene.sun.pos * 1);
    






    //?CASCADE CALC

    float cascadeSplits[3];

    float nearClip = 0.01f;
    float farClip  = 100.f;
    float clipRange = farClip - nearClip;

    float minZ = nearClip;
    float maxZ = nearClip + clipRange;

    float range = maxZ - minZ;
    float ratio = maxZ / minZ;

    float cascadeSplitLambda = 0.95f;

    for (uint32_t i = 0; i < 3; i++) {
        float p = (i + 1) / static_cast<float>(3);
        float log = minZ * std::pow(ratio, p);
        float uniform = minZ + range * p;
        float d = cascadeSplitLambda * (log - uniform) + uniform;
        cascadeSplits[i] = (d - nearClip) / clipRange;
    }

    resources.common_pushConsts2.cascadeSplits = { 
        cascadeSplits[0] ,
        cascadeSplits[1] ,
        cascadeSplits[2] ,
    };
    















    static float t = 0;
    t+=(f32)dt;
    resources.common_pushConsts.time = t;

    resources.common_pushConsts.sun    = {};//scene.sun.GetOrthographic(0) * scene.sun.GetView(); //BIAS * 
    resources.shadow.pushConsts.sunView = scene.sun.GetView();
    //resources.shadow.pushConsts.sun[1] = scene.sun.GetOrthographic(1) * scene.sun.GetView();
    //resources.shadow.pushConsts.sun[2] = scene.sun.GetOrthographic(2) * scene.sun.GetView();

    resources.common_pushConsts.sunBias= BIAS * scene.sun.GetOrthographic(0) * scene.sun.GetView();
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
    if (hostRes.terrain.settings.baked){
        if (!resources.terrain.vbo.IsBaked())
            resources.terrain.vbo.Bake(commands.cmdPool);
    }
    else
    {
        resources.terrain.vbo.activeBuffer = &resources.terrain.vbo.cpuBuffer;
        FOR_ARRAY(hostRes.terrain.settings.dirtyQuadrants, i){
            const auto quadrantIdx = hostRes.terrain.settings.dirtyQuadrants[i];
            resources.terrain.vbo.UpdateGroup(quadrantIdx, hostRes.terrain.GetQuadrant(quadrantIdx).verts);
        }
    }
    

}

} //ns