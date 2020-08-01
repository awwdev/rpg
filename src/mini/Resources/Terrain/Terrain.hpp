//https://github.com/awwdev

#pragma once
#include "mini/Utils/Structs.hpp"
#include "mini/Box/Array.hpp"
#include "mini/ECS/ECS.hpp"
#include "mini/Rendering/Camera.hpp"
#include "mini/Resources/Terrain/Terrain_Quadrant.hpp"
#include "mini/Resources/Terrain/Terrain_Serialization.hpp"

namespace mini::res2
{
    template<auto QUAD_COUNT, auto QUADRANT_LENGTH, auto QUADRANT_COUNT_T>
    struct Terrain
    {
        using QUADRANT_T = Quadrant<QUAD_COUNT, QUADRANT_LENGTH>;

        static constexpr auto QUADRANT_COUNT = QUADRANT_COUNT_T;
        static constexpr auto TOTAL_LENGTH   = QUADRANT_COUNT * QUADRANT_LENGTH;
        QUADRANT_T quadrants [QUADRANT_COUNT][QUADRANT_COUNT];

        struct Editing 
        {
            box::Array<idx_t, 6> draggingVertIndices;
            bool isDragging = false;
            f32  yDragPoint = 0;
            f32  dragScale = 0.05f;
        } editing;
        
        u32 editingQuadrantIdx = 0;

        const QUADRANT_T& GetEditingQuadrant() const
        {       
            const auto x = editingQuadrantIdx % QUADRANT_COUNT;
            const auto z = editingQuadrantIdx / QUADRANT_COUNT;
            return quadrants[z][x];
        }

        QUADRANT_T& GetEditingQuadrant()
        {       
            const auto x = editingQuadrantIdx % QUADRANT_COUNT;
            const auto z = editingQuadrantIdx / QUADRANT_COUNT;
            return quadrants[z][x];
        }

        void Create()
        {
            for(idx_t z = 0; z < QUADRANT_COUNT; ++z) {
            for(idx_t x = 0; x < QUADRANT_COUNT; ++x) {
                quadrants[z][x].Create(
                    (f32)z * QUADRANT_LENGTH - TOTAL_LENGTH * 0.5f, 
                    (f32)x * QUADRANT_LENGTH - TOTAL_LENGTH * 0.5f);
            }}
        }

        void TestIntersectionAndPressed(const rendering::Camera& camera)
        {
            auto& quadrant = GetEditingQuadrant();
            const auto ray = camera.ScreenRay();

            for(idx_t i = 0; i < quadrant.VERT_COUNT_TOTAL; i+=3)
            {
                auto& v0 = quadrant.verts[i+0].pos;
                auto& v1 = quadrant.verts[i+1].pos;
                auto& v2 = quadrant.verts[i+2].pos;

                const box::Optional<utils::Intersection> intersection = 
                    utils::RayTriangleIntersection(camera.pos, ray, v0, v1, v2);
                if (intersection)
                {
                    const auto ix = intersection->pos[X];
                    const auto iy = intersection->pos[Y];
                    const auto iz = intersection->pos[Z];
                    //visualize
                    const auto closestVertex = intersection->GetClosestVertex(i);

                    if (wnd::HasEvent<wnd::Mouse_ButtonLeft, wnd::Pressed>()){
                        editing.draggingVertIndices.Clear();
                        const auto corner   = quadrant.GetCornerByVertex(closestVertex);
                        const auto vertices = quadrant.GetVerticesByCorner(corner);
                        editing.draggingVertIndices.AppendArray(vertices);
                        editing.yDragPoint = (f32)wnd::global::mouse_wy;
                        editing.isDragging = true;
                    }
                }
            }
        }

        void Dragging()
        {
            LOG("dragging");

            using namespace math;
            auto& quadrant = GetEditingQuadrant();

            const f32 yDelta = wnd::global::mouse_wy - editing.yDragPoint;
            editing.yDragPoint = (f32)wnd::global::mouse_wy;
            
            const auto& vertIndices = editing.draggingVertIndices;
            FOR_ARRAY(vertIndices, i){
                const auto idx = vertIndices[i];
                quadrant.verts[idx].pos[Y] += yDelta * editing.dragScale;
            }
        }

        void Update(const double dt, const rendering::Camera& camera, ecs::ECS& ecs)
        {   
            if (wnd::HasEvent<wnd::Mouse_ButtonLeft, wnd::Released>())
                editing.isDragging = false;

            if (editing.isDragging == false)
                TestIntersectionAndPressed(camera);

            if (editing.isDragging == true)
                Dragging();
        }
    };

}//ns