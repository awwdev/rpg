//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources/Resources.hpp"
#include "mini/Vulkan/Commands.hpp"
#include "mini/App/Scene.hpp"
#include "mini/Resources/HostResources.hpp"

namespace mini::vk {

const utils::Mat4f BIAS { 
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.5, 0.5, 0.0, 1.0 
};

/*
inline void CreateCascades(utils::Mat4f (&cascades)[2], const app::GameScene& scene)
{
    using namespace utils;

    cascades[0] = scene.sun.GetOrthographic(0) * scene.sun.GetView();
    cascades[1] = scene.sun.GetOrthographic(1) * scene.sun.GetView();
    //cascades[2] = scene.sun.GetOrthographic(2) * scene.sun.GetView();

    constexpr auto CASCADES_MAX = 2;

    float cascadeSplits[CASCADES_MAX];

    float nearClip  = 0.01f;
    float farClip   = 100.f;
    float clipRange = farClip - nearClip;

    float minZ = nearClip;
    float maxZ = nearClip + clipRange;

    float range = maxZ - minZ;
    float ratio = maxZ / minZ;

    for (uint32_t i = 0; i < CASCADES_MAX; i++) {
        float p = (i + 1) / static_cast<float>(CASCADES_MAX);
        float log = minZ * std::pow(ratio, p);
        float uniform = minZ + range * p;
        float cascadeSplitLambda = 0.95f;
        float d = cascadeSplitLambda * (log - uniform) + uniform;
        cascadeSplits[i] = (d - nearClip) / clipRange;
    }

    float lastSplitDist = 0.0;
    for (uint32_t i = 0; i < CASCADES_MAX; i++) {
        float splitDist = cascadeSplits[i];
        utils::Vec3f frustumCorners[8] = {
            {  1.0f,  1.0f, -1.0f },
            {  1.0f,  1.0f, -1.0f },
            {  1.0f, -1.0f, -1.0f },
            {  1.0f, -1.0f, -1.0f },
            {  1.0f,  1.0f,  1.0f },
            {  1.0f,  1.0f,  1.0f },
            {  1.0f, -1.0f,  1.0f },
            {  1.0f, -1.0f,  1.0f },
        };

        auto invCam  = utils::Inverse(scene.editorController.camera.perspective * scene.editorController.camera.view);
        for (uint32_t i = 0; i < 8; i++) {
            utils::Vec4f invCorner = invCam * utils::Vec4f { frustumCorners[i][X], frustumCorners[i][Y], frustumCorners[i][Z], 1.0f };
            auto c = invCorner / invCorner[W];
            frustumCorners[i] = { c[X], c[Y], c[Z] };
        }

        for (uint32_t i = 0; i < 4; i++) {
            utils::Vec3f dist = frustumCorners[i + 4] - frustumCorners[i];
            frustumCorners[i + 4] = frustumCorners[i] + (dist * splitDist);
            frustumCorners[i] = frustumCorners[i] + (dist * lastSplitDist);
        }

        utils::Vec3f frustumCenter {};
        for (uint32_t i = 0; i < 8; i++) {
            frustumCenter = frustumCenter + frustumCorners[i];
        }
        frustumCenter = frustumCenter / 8.0f;

        float radius = 0.0f;
        for (uint32_t i = 0; i < 8; i++) {
            float distance = utils::Magnitude(frustumCorners[i] - frustumCenter);
            radius = utils::Max(radius, distance);
        }
        //radius = std::ceil(radius * 16.0f) / 16.0f;

        utils::Vec3f maxExtents = { radius, radius, radius };
        utils::Vec3f minExtents = maxExtents * -1;

        utils::Vec3f lightDir = utils::Normalize(scene.sun.pos * -1);
        utils::Mat4f lightViewMatrix  = utils::LookAt(frustumCenter - lightDir * -minExtents[Z], frustumCenter);

        float l = minExtents[X];
        float r = maxExtents[X];
        float b = minExtents[Y];
        float t = maxExtents[Y];
        float f = maxExtents[Z] - minExtents[Z];
        float n = 0;
        float A =   2 / (r-l);
        float B =   2 / (t-b);
        float C = - 2 / (f-n);
        float x = - (r + l) / (r - l);
        float y = - (t + b) / (t - b);
        float z = - (f + n) / (f - n);
        utils::Mat4f lightOrthoMatrix = {
            A, 0, 0, 0,  
            0, B, 0, 0,  
            0, 0, C, 0,  
            x, y, z, 1,  
        };

        /*
        const float S = radius;
        const float D = 0.00001f; 
        const float Z = 0.01f;
        utils::Mat4f lightOrthoMatrix = {
            S, 0, 0, 0,
            0, S, 0, 0,
            0, 0, D, 0,
            0, 0, Z, 1,
        };
        //cascades[i].splitDepth = (camera.getNearClip() + splitDist * clipRange) * -1.0f;

        cascades[i] = lightOrthoMatrix * lightViewMatrix;

        lastSplitDist = cascadeSplits[i];
        }
}
*/
inline void CreateCascades2(utils::Mat4f (&cascades)[2], const app::GameScene& scene, rendering::Terrain_UniformData& uboMeta)
{
    using namespace utils;

    uboMeta = {
        .camProj = (app::global::inputMode == app::global::PlayMode ? scene.playerController.camera.perspective : scene.editorController.camera.perspective),
        .camView = (app::global::inputMode == app::global::PlayMode ? scene.playerController.camera.view        : scene.editorController.camera.view),
        .sunView = scene.sun.GetView(),
        .sunDir  = utils::Normalize(scene.sun.pos),
        .cascadeFadeDist = 100
    };

    auto cascadeZoom0 = 1;
    auto cascadeZoom1 = 5;

    auto& camera = scene.editorController.camera;

    auto farPlane = -100.f;
    auto cascadeCenter0 = Vec2f{ camera.position[X], camera.position[Z] }  + (Normalize(Vec2f{ camera.rotation[X], camera.rotation[Z] }) * farPlane * 0.3f);
    auto cascadeCenter1 = Vec2f{ camera.position[X], camera.position[Z] }  + (Normalize(Vec2f{ camera.rotation[X], camera.rotation[Z] }) * farPlane * 0.7f);
    
    auto sunDir  = utils::Normalize(scene.sun.pos) * -1;
    auto sunPos0 = utils::Vec3f{ cascadeCenter0[X], scene.sun.pos[Y], cascadeCenter0[Z] } + (sunDir * -1 * cascadeZoom0);
    auto sunPos1 = utils::Vec3f{ cascadeCenter1[X], scene.sun.pos[Y], cascadeCenter1[Z] } + (sunDir * -1 * cascadeZoom1);

    auto view0 = utils::LookAt(sunPos0, utils::Vec3f{ cascadeCenter0[X], scene.sun.pos[Y], cascadeCenter0[Z] });
    auto view1 = utils::LookAt(sunPos1, utils::Vec3f{ cascadeCenter1[X], scene.sun.pos[Y], cascadeCenter1[Z] });

    float S;
    const float D = 0.00001f; 
    const float Z = 0.01f;

    S  = 0.01f;
    utils::Mat4f ortho0 ={
        S, 0, 0, 0,
        0, S, 0, 0,
        0, 0, D, 0,
        0, 0, Z, 1,
    };

    S  = 0.002f;
    utils::Mat4f ortho1 ={
        S, 0, 0, 0,
        0, S, 0, 0,
        0, 0, D, 0,
        0, 0, Z, 1,
    };

    cascades[1] = ortho0 * view0;
    cascades[0] = ortho1 * view1;
}

inline void UpdateVkResources_GameScene(VkResources& resources, const app::GameScene& scene, res::HostResources& hostRes, double dt, Commands& commands)
{
    

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
    

    //? UBO META TEST
    utils::Mat4f cascades [2]{};
    rendering::Terrain_UniformData uboMeta {};
    CreateCascades2(cascades, scene, uboMeta);

    uboMeta.sunProjCasc[0] = BIAS * cascades[0];
    uboMeta.sunProjCasc[1] = BIAS * cascades[1];
    resources.terrain.uboMeta.Store(uboMeta);

    resources.shadow.pushConsts.sunCasc[0] = cascades[0];
    resources.shadow.pushConsts.sunCasc[1] = cascades[1];




    static float t = 0;
    t+=(f32)dt;
    resources.common_pushConsts.time = t;

    resources.common_pushConsts.sun    = {};//scene.sun.GetOrthographic(0) * scene.sun.GetView(); //BIAS * 
    //resources.shadow.pushConsts.sunView = scene.sun.GetView();
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