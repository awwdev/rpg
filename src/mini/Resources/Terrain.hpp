//https://github.com/awwdev

#pragma once
#include "mini/Utils/PrimitiveMeshes.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/Resources/Mesh.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/ECS/ECS.hpp"
#include "mini/Rendering/Camera.hpp"
#include "mini/Window/AppEvents.hpp"
#include "mini/Box/Array.hpp"

//TODO: AABB to check which quadrant
//TODO: exapandable
//TODO: serialize

namespace mini::res
{
    struct Quadrant
    {
        static constexpr auto QUAD_COUNT = 10;
        static constexpr auto QUAD_COUNT_TOTAL = QUAD_COUNT * QUAD_COUNT;
        static constexpr auto VERT_COUNT_TOTAL = QUAD_COUNT_TOTAL * 6;
        static constexpr auto SIZE  = 20;

        const f32 quadrantX;
        const f32 quadrantY;
        utils::Common_Vertex verts [VERT_COUNT_TOTAL];

        box::Array<u32, 6> qCorners [QUAD_COUNT+1][QUAD_COUNT+1];
        u32 GetCornerByVertex(const u32 vIdx)
        {
            const auto normIdx = (u32)(vIdx % 6);
            const auto quadIdx = (u32)(vIdx / 6);
            const auto quadRow = (u32)(quadIdx / QUAD_COUNT);

            if (normIdx == 0 || normIdx == 3)        return quadIdx + quadRow + 0;
            else if (normIdx == 1)                   return quadIdx + quadRow + 1;
            else if (normIdx == 2 || normIdx == 4)   return quadIdx + quadRow + 1 + QUAD_COUNT + 1;
            else /*if (normIdx == 5)*/               return quadIdx + quadRow + 0 + QUAD_COUNT + 1;
        }

        void Create(const math::Vec4f& col = { 0.1f, 0.7f, 0.1f, 1 })
        {
            const auto quadSize = SIZE  / QUAD_COUNT;

            for(u8 z = 0; z < QUAD_COUNT; ++z) {
            for(u8 x = 0; x < QUAD_COUNT; ++x) { 
                const auto idx = (z * QUAD_COUNT + x) * 6;
                verts[idx + 0] = { {  quadrantX + 0.0f * quadSize + x * quadSize, 0,  quadrantY + 0.0f * quadSize + z * quadSize, 1 }, { 0, -1, 0, 1}, col, {} };
                verts[idx + 1] = { {  quadrantX + 1.0f * quadSize + x * quadSize, 0,  quadrantY + 0.0f * quadSize + z * quadSize, 1 }, { 0, -1, 0, 1}, col, {} };
                verts[idx + 2] = { {  quadrantX + 1.0f * quadSize + x * quadSize, 0,  quadrantY + 1.0f * quadSize + z * quadSize, 1 }, { 0, -1, 0, 1}, col, {} };
                verts[idx + 3] = { {  quadrantX + 0.0f * quadSize + x * quadSize, 0,  quadrantY + 0.0f * quadSize + z * quadSize, 1 }, { 0, -1, 0, 1}, col, {} };
                verts[idx + 4] = { {  quadrantX + 1.0f * quadSize + x * quadSize, 0,  quadrantY + 1.0f * quadSize + z * quadSize, 1 }, { 0, -1, 0, 1}, col, {} };
                verts[idx + 5] = { {  quadrantX + 0.0f * quadSize + x * quadSize, 0,  quadrantY + 1.0f * quadSize + z * quadSize, 1 }, { 0, -1, 0, 1}, col, {} };
            }}

            //TODO: bitset instead of array
            const auto CORNER_COUNT = QUAD_COUNT + 1;
            for(u8 z = 0; z < CORNER_COUNT; ++z) {
            for(u8 x = 0; x < CORNER_COUNT; ++x) { 

                auto& corner = qCorners[z][x];
                const auto cIdx = x*6 + z*(QUAD_COUNT*6);

                if (x != CORNER_COUNT - 1){
                    if (z != CORNER_COUNT - 1) 
                    {
                        corner.Append(cIdx);
                        corner.Append(cIdx + 3);
                        if (x > 0)  corner.Append(cIdx - 5);
                    }
                    
                    if (z > 0)  corner.Append(cIdx - (QUAD_COUNT*6) + 5);
                    if (x > 0 && z > 0) {      
                        corner.Append(cIdx - (QUAD_COUNT*6) - 2);
                        corner.Append(cIdx - (QUAD_COUNT*6) - 4);
                    }
                }  
                if (x == CORNER_COUNT - 1) {
                    const auto cIdx_ = (x-1)*6 + z*(QUAD_COUNT*6) + 1;
                    if (z != CORNER_COUNT - 1) 
                        corner.Append(cIdx_);
                    if (z > 0) {
                        corner.Append(cIdx_ - (QUAD_COUNT*6) + 1);
                        corner.Append(cIdx_ - (QUAD_COUNT*6) + 3);
                    }
                }
                    

            }}

            //for(u8 z = 0; z < CORNER_COUNT; ++z) {
            //for(u8 x = 0; x < CORNER_COUNT; ++x) { 
            //    box::PrintArray(qCorners[z][x]);
            //} LOG("//////////////");}
        }


    };

    struct Terrain
    {
        Quadrant quadrants [1] = {
            { -Quadrant::SIZE*0.5f, -Quadrant::SIZE*0.5 },
            //{ Quadrant::SIZE, 0 },
            //{ 0,              Quadrant::SIZE },
            //{ Quadrant::SIZE, Quadrant::SIZE },
        };

        ecs::ID gizmoID = 0;
        const float S = 1.f; //gizmo cube scale

        f32 yDragPoint = 0;
        s32 draggedVertex = -1;
        u32 corner = 0;
        enum CloseVertex { V0, V1, V2 } closeVertex = V0;
        u32 triangle = 0;

        void Create(ecs::ECS& ecs)
        {
            quadrants[0].Create({ 0.1f, 0.7f, 0.1f, 1 });
            //quadrants[1].Create({ 0.7f, 0.1f, 0.1f, 1 });
            //quadrants[2].Create({ 0.1f, 0.1f, 0.7f, 1 });
            //quadrants[3].Create({ 0.7f, 0.7f, 0.7f, 1 });

            //GIZMO CUBE
            //to display snap
            {
                constexpr float x =  0; 
                constexpr float y =  0;
                constexpr float z =  0;
                gizmoID = ecs.AddEntity();
                const math::Mat4f pos = math::Mat4f{
                    S, 0, 0, 0,
                    0, S, 0, 0,
                    0, 0, S, 0,
                    x, y, z, 1,
                };
                ecs.arrays.AddComponent<ecs::ComponentType::Transform> (gizmoID, pos);
                ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(gizmoID, res::MeshType::PrimitiveCube);
            }
        }

        void Update(const double dt, const rendering::Camera& camera, ecs::ECS& ecs)
        {
            auto& quadrant = quadrants[0];

            //INTERSECTION
            using namespace utils;
            using namespace math;

            const auto ray = camera.ScreenRay();
            

            for(auto i = 0; i < quadrant.VERT_COUNT_TOTAL; i+=3)
            {
                auto& v0 = quadrant.verts[i+0].pos;
                auto& v1 = quadrant.verts[i+1].pos;
                auto& v2 = quadrant.verts[i+2].pos;
                const auto intersection = utils::RayTriangleIntersection(
                    camera.pos,
                    ray,
                    { v0[X], v0[Y], v0[Z] },
                    { v1[X], v1[Y], v1[Z] },
                    { v2[X], v2[Y], v2[Z] }
                );

                if (intersection)
                {
                    auto x = intersection->pos[X];
                    auto y = intersection->pos[Y];
                    auto z = intersection->pos[Z];
                    triangle = i / 3;

                    //TODO: could be written better:
                    if      (intersection->u > 0.5) closeVertex = V1;
                    else if (intersection->v > 0.5) closeVertex = V2;
                    else if (intersection->u > 0.25 && intersection->v > 0.25)
                    {
                        if  (intersection->u > intersection->v) closeVertex = V1;
                        else closeVertex = V2;
                    }

                    if (closeVertex == V0)
                    {
                        x = v0[X];
                        y = v0[Y];
                        z = v0[Z];
                    }
                    if (closeVertex == V1)
                    {
                        x = v1[X];
                        y = v1[Y];
                        z = v1[Z];
                    }
                    if (closeVertex == V2)
                    {
                        x = v2[X];
                        y = v2[Y];
                        z = v2[Z];
                    }

                    //display snap
                    //auto& cubeTrans = ecs.arrays.transforms.Get(gizmoID);
                    //cubeTrans.transform = {
                    //    S, 0, 0, 0,
                    //    0, S, 0, 0,
                    //    0, 0, S, 0,
                    //    x, y, z 1,
                    //};
                
                    if (wnd::HasEvent<wnd::Mouse_ButtonLeft, wnd::Pressed>())
                    {
                        yDragPoint = (f32)wnd::global::mouse_window_y;
                        if (closeVertex == V0) draggedVertex = i+0;
                        if (closeVertex == V1) draggedVertex = i+1;
                        if (closeVertex == V2) draggedVertex = i+2;
                        corner = quadrant.GetCornerByVertex(draggedVertex);
                    }
                    
                    break;
                }

            }//for end

            const auto mouseLeftButton = wnd::global::events[wnd::Mouse_ButtonLeft];
            if (mouseLeftButton == wnd::Released)
            {
                //Recalculate normals
                const auto cx = corner % (quadrant.QUAD_COUNT + 1);
                const auto cy = corner / (quadrant.QUAD_COUNT + 1);
                FOR_ARRAY(quadrant.qCorners[cy][cx], i)
                {
                    const auto vIdx = quadrant.qCorners[cy][cx][i];
                    const auto tris = vIdx/3;

                    auto& v0  = quadrant.verts[tris*3 + 0];
                    auto& v1  = quadrant.verts[tris*3 + 1];
                    auto& v2  = quadrant.verts[tris*3 + 2];
                    const auto p0   = math::TruncateVec4(v0.pos);
                    const auto p1   = math::TruncateVec4(v1.pos);
                    const auto p2   = math::TruncateVec4(v2.pos);
                    const auto vec1 = math::Normalize(p0 - p1);
                    const auto vec2 = math::Normalize(p0 - p2);
                    const auto norm = math::Cross(vec1, vec2);
                    v0.nor = v1.nor = v2.nor = MakeHomoVec(norm);
                }

                draggedVertex = -1;
            }

            if (draggedVertex >= 0)
            {
                const auto yDragDelta = (f32)wnd::global::mouse_window_y - yDragPoint;
                constexpr f32 dragScale = 0.01f;
                
                const auto cx = corner % (quadrant.QUAD_COUNT + 1);
                const auto cy = corner / (quadrant.QUAD_COUNT + 1);
                FOR_ARRAY(quadrant.qCorners[cy][cx], i)
                {
                    const auto vIdx = quadrant.qCorners[cy][cx][i];
                    auto& v = quadrant.verts[vIdx].pos;
                    v[Y] += yDragDelta * dragScale;
                }
                yDragPoint = (f32)wnd::global::mouse_window_y;

                
            }
        }

    };
    
}//ns