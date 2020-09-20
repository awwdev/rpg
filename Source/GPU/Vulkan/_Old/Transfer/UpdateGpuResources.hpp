//https://github.com/awwdev

#pragma once

#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/_Old/Resources/Resources.hpp"
#include "GPU/Vulkan/Meta/Commands.hpp"
#include "App/Scene.hpp"
#include "Resources/CpuResources.hpp"

namespace rpg::gpu::vuk {

const com::Mat4f BIAS { 
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.5, 0.5, 0.0, 1.0 
};

inline void CreateCascades2(com::Mat4f (&cascades)[3], const app::GameScene& scene, gpu::Terrain_UniformData& uboMeta)
{
    using namespace com;

    auto shadowDist0 = scene.renderData.maxShadowDist[0];
    auto shadowDist1 = scene.renderData.maxShadowDist[1];
    auto shadowDist2 = scene.renderData.maxShadowDist[21];
    shadowDist0 =  10;
    shadowDist1 =  90;
    shadowDist2 = 500;

    uboMeta = {
        .camProj = (app::glo::inputMode == app::glo::PlayMode ? scene.playerController.camera.perspective : scene.editorController.camera.perspective),
        .camView = (app::glo::inputMode == app::glo::PlayMode ? scene.playerController.camera.view        : scene.editorController.camera.view),
        .sunView = scene.sun.GetView(),
        .sunDir  = com::Normalize(scene.sun.pos),
        .cascadeFadeDist0 = shadowDist0,
        .cascadeFadeDist1 = shadowDist1,
        .cascadeFadeDist2 = shadowDist2,
    };

    //auto farPlane = shadowDist;
    auto sunDir   = com::Normalize(scene.sun.pos);
    auto sunDist  = 10;

    Vec3f invCameraPos;
    invCameraPos = { -scene.editorController.camera.position[X], scene.editorController.camera.position[Y], -scene.editorController.camera.position[Z] };
    if (app::glo::inputMode == app::glo::PlayMode)
        invCameraPos = { -scene.playerController.camera.view[3][0], scene.playerController.camera.view[3][1], -scene.playerController.camera.view[3][2] };

    //Vec3f cameraDir    { Sin(camera.rotation[Y] * 3.14f/180.f), 0, Cos(camera.rotation[Y] * 3.14f/180.f) };

    auto cascadePos0 = invCameraPos;
    auto cascadePos1 = invCameraPos;// + (Normalize(cameraDir) * -farPlane);
    auto cascadePos2 = invCameraPos;// + (Normalize(cameraDir) * -farPlane);

    auto sunPos0 = cascadePos0 + (sunDir * sunDist);
    auto sunPos1 = cascadePos1 + (sunDir * sunDist);
    auto sunPos2 = cascadePos2 + (sunDir * sunDist);

    auto view0 = com::LookAt(sunPos0, cascadePos0);
    auto view1 = com::LookAt(sunPos1, cascadePos1);
    auto view2 = com::LookAt(sunPos2, cascadePos2);

    float S;
    const float D = 0.00001f; 
    const float Z = 0.02f;

    

    S = scene.renderData.cascadeZoom[0];
    S = 0.075f;
    com::Mat4f ortho0 ={
        S, 0, 0, 0,
        0, S, 0, 0,
        0, 0, D, 0,
        0, 0, Z, 1,
    };

    S = scene.renderData.cascadeZoom[1];
    S = 0.014f;
    com::Mat4f ortho1 ={
        S, 0, 0, 0,
        0, S, 0, 0,
        0, 0, D, 0,
        0, 0, Z, 1,
    };

    S = scene.renderData.cascadeZoom[2];
    S = 0.0019f;
    com::Mat4f ortho2 ={
        S, 0, 0, 0,
        0, S, 0, 0,
        0, 0, D, 0,
        0, 0, Z, 1,
    };

    cascades[0] = ortho0 * view0;
    cascades[1] = ortho1 * view1;
    cascades[2] = ortho2 * view2;
}

inline void UpdateVkResources_GameScene(VkResources& resources, const app::GameScene& scene, res::CpuResources& hostRes, double dt, Commands& commands)
{
    //? POST
    resources.post.vbo.Reset();
    resources.post.vbo.StoreGroup(scene.renderData.gui_vbo_blur.Data(), scene.renderData.gui_vbo_blur.count);

    //? UI
    resources.ui.pushConsts.wnd_w = wnd::glo::window_w;
    resources.ui.pushConsts.wnd_h = wnd::glo::window_h;
    resources.ui.ubo.Reset();
    resources.ui.ubo.Store(scene.renderData.uboData_gui_text);
















    //resources.common_pushConsts.camera = scene.camera.GetOrthographic() * scene.sun.GetView();
    //resources.common_pushConsts.camera = scene.camera.GetPerspective()  * scene.playerController.GetView(scene.ecs);
    //resources.common_pushConsts.camera = scene.camera.GetPerspective()  * scene.camera.GetView();

    /*
    //TODO: this should be handled somewhere else which is active
    if (app::glo::inputMode == app::glo::PlayMode){
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
    resources.common_pushConsts2.sunDir  = com::Normalize(scene.sun.pos * 1);
    

    //? UBO META TEST
    static com::Mat4f cascades [3]{};
    gpu::Terrain_UniformData uboMeta {};
    CreateCascades2(cascades, scene, uboMeta);

    uboMeta.sunProjCasc[0] = BIAS * cascades[0];
    uboMeta.sunProjCasc[1] = BIAS * cascades[1];
    uboMeta.sunProjCasc[2] = BIAS * cascades[2];
    resources.terrain.uboMeta.Store(uboMeta);

    resources.shadow.pushConsts.sunCasc[0] = cascades[0];
    resources.shadow.pushConsts.sunCasc[1] = cascades[1];
    resources.shadow.pushConsts.sunCasc[2] = cascades[2];




    static float t = 0;
    t+=(f32)dt;
    resources.common_pushConsts.time = t;
    resources.common_pushConsts.sun    = {};


    resources.common_pushConsts.sunBias= BIAS * scene.sun.GetOrthographic(0) * scene.sun.GetView();
    resources.common_pushConsts.sunDir = com::Normalize(scene.sun.pos * 1);
    
    resources.sky.pushConsts.topColor = { 0.1f, 0.1f, 1.0f, 1 };
    resources.sky.pushConsts.botColor = { 1.0f, 1.0f, 1.0f, 1 };
    */

   

    /*
    resources.common.ubo.Clear();
    resources.common.ubo.Store(scene.renderData.common_ubo);

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
    */
    
}

} //ns