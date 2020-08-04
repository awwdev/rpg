//https://github.com/awwdev

#pragma once
#include "mini/Utils/Structs.hpp"
#include "mini/Box/Array.hpp"
#include "mini/ECS/ECS.hpp"
#include "mini/Rendering/Cameras.hpp"
#include "mini/Resources/Terrain/Terrain_Quadrant.hpp"

#include <fstream>

namespace mini::res
{
    template<auto QUAD_COUNT, auto QUADRANT_LENGTH, auto QUADRANT_COUNT_T>
    struct Terrain
    {
        using QUADRANT_T = Quadrant<QUAD_COUNT, QUADRANT_LENGTH>;

        static constexpr auto QUADRANT_COUNT = QUADRANT_COUNT_T;
        static constexpr auto TOTAL_QUADRANT_COUNT = QUADRANT_COUNT * QUADRANT_COUNT;
        static constexpr auto TOTAL_LENGTH   = QUADRANT_COUNT * QUADRANT_LENGTH;
        QUADRANT_T quadrants [QUADRANT_COUNT][QUADRANT_COUNT];

        struct 
        {
            box::Array<idx_t, 6> draggingVertIndices;
            bool isDragging = false;
            f32  yDragPoint = 0;
            f32  dragScale = 0.05f;
            u32  quadrantIdx = 0;
            box::Array<idx_t, TOTAL_QUADRANT_COUNT> dirtyQuadrants;
        } editing;

        const QUADRANT_T& GetQuadrant(const idx_t i) const
        {
            const auto x = i % QUADRANT_COUNT;
            const auto z = i / QUADRANT_COUNT;
            return quadrants[z][x];
        }

        QUADRANT_T& GetQuadrant(const idx_t i)
        {
            const auto x = i % QUADRANT_COUNT;
            const auto z = i / QUADRANT_COUNT;
            return quadrants[z][x];
        }

        idx_t GetQuadrantIndex(const idx_t z, const idx_t x) const
        {
            return z * QUADRANT_COUNT + x;
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

        void Update(const double dt, const rendering::EgoCamera& camera, ecs::ECS& ecs)
        {   
            if (app::global::inputMode != app::global::UI_Mode)
                return;

            editing.dirtyQuadrants.Clear();

            if (wnd::HasEvent<wnd::Mouse_ButtonLeft, wnd::Released>())
                editing.isDragging = false;

            if (editing.isDragging == false)
                TestIntersectionAndPressed(camera);

            if (editing.isDragging == true)
                Dragging();

            if (wnd::HasEvent<wnd::F5, wnd::Pressed>())
                Save();
            if (wnd::HasEvent<wnd::F6, wnd::Pressed>())
                Load();
            if (wnd::HasEvent<wnd::F7, wnd::Pressed>())
                Stiching();

            if (wnd::HasEvent<wnd::N0, wnd::Pressed>()) editing.quadrantIdx = 0;
            if (wnd::HasEvent<wnd::N1, wnd::Pressed>()) editing.quadrantIdx = 1;
            if (wnd::HasEvent<wnd::N2, wnd::Pressed>()) editing.quadrantIdx = 2;
            if (wnd::HasEvent<wnd::N3, wnd::Pressed>()) editing.quadrantIdx = 3;
        }

        void TestIntersectionAndPressed(const rendering::EgoCamera& camera)
        {
            auto& quadrant = GetQuadrant(editing.quadrantIdx);
            const auto ray = ScreenRay(camera);

            for(idx_t i = 0; i < quadrant.VERT_COUNT_TOTAL; i+=3)
            {
                auto& v0 = quadrant.verts[i+0].pos;
                auto& v1 = quadrant.verts[i+1].pos;
                auto& v2 = quadrant.verts[i+2].pos;

                const box::Optional<utils::Intersection> intersection = 
                    utils::RayTriangleIntersection(camera.position, ray, v0, v1, v2);
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
            using namespace math;
            auto& quadrant = GetQuadrant(editing.quadrantIdx);
            editing.dirtyQuadrants.Append(editing.quadrantIdx);

            const f32 yDelta = wnd::global::mouse_wy - editing.yDragPoint;
            editing.yDragPoint = (f32)wnd::global::mouse_wy;
            
            const auto& vertIndices = editing.draggingVertIndices;
            FOR_ARRAY(vertIndices, i){
                const auto idx = vertIndices[i];
                quadrant.verts[idx].pos[Y] += yDelta * editing.dragScale;
            }
        }

        void Stiching()
        {
            LOG("stiching terrain");
            //current coord of active editing quadrant
            const auto z = editing.quadrantIdx / QUADRANT_COUNT;
            const auto x = editing.quadrantIdx % QUADRANT_COUNT;
            editing.dirtyQuadrants.Append(editing.quadrantIdx);

            //neighbors
            const bool hasNeighborRight  = x < QUADRANT_COUNT - 1;
            const bool hasNeighborBottom = z < QUADRANT_COUNT - 1;
            const bool hasNeighborLeft   = x > 0;
            const bool hasNeighbofTop    = z > 0;

            if (hasNeighborRight)
            {
                auto& quadrant = quadrants[z][x]; //current active one
                auto& neighborQuadrant = quadrants[z][x+1];
                const auto quadrantIdxNeighbor = GetQuadrantIndex(z, x+1);
                editing.dirtyQuadrants.Append(quadrantIdxNeighbor);

                for(idx_t z = 0; z < quadrant.CORNER_COUNT; ++z){
                    auto& edgeVerts         = quadrant.corners[z][quadrant.CORNER_COUNT - 1]; //right edge
                    auto& edgeVertsNeighbor = neighborQuadrant.corners[z][0]; //left edge

                    const auto averagePos = [&]
                    { 
                        auto  vIdx         = edgeVerts[0];
                        auto  vIdxNeighbor = edgeVertsNeighbor[0];
                        auto& pos          = quadrant.verts[vIdx].pos;
                        auto& posNeighbor  = neighborQuadrant.verts[vIdxNeighbor].pos;

                        return (pos + posNeighbor) * 0.5f;  
                    }();

                    FOR_ARRAY(edgeVerts , i) { 
                        auto vIdx = edgeVerts[i];
                        auto& pos = quadrant.verts[vIdx].pos;
                        pos = averagePos;
                    }

                    FOR_ARRAY(edgeVertsNeighbor , i) { 
                        auto vIdx = edgeVertsNeighbor[i];
                        auto& pos = neighborQuadrant.verts[vIdx].pos;
                        pos = averagePos;
                    }
                }
            }

            if (hasNeighborBottom)
            {
                auto& quadrant = quadrants[z][x]; //current active one
                auto& neighborQuadrant = quadrants[z+1][x];
                const auto quadrantIdxNeighbor = GetQuadrantIndex(z+1, x);
                editing.dirtyQuadrants.Append(quadrantIdxNeighbor);

                for(idx_t x = 0; x < quadrant.CORNER_COUNT; ++x){
                    auto& edgeVerts         = quadrant.corners[quadrant.CORNER_COUNT - 1][x]; //bottom edge
                    auto& edgeVertsNeighbor = neighborQuadrant.corners[0][x]; //top edge

                    const auto averagePos = [&]
                    { 
                        auto  vIdx         = edgeVerts[0];
                        auto  vIdxNeighbor = edgeVertsNeighbor[0];
                        auto& pos          = quadrant.verts[vIdx].pos;
                        auto& posNeighbor  = neighborQuadrant.verts[vIdxNeighbor].pos;

                        return (pos + posNeighbor) * 0.5f;  
                    }();

                    FOR_ARRAY(edgeVerts , i) { 
                        auto vIdx = edgeVerts[i];
                        auto& pos = quadrant.verts[vIdx].pos;
                        pos = averagePos;
                    }

                    FOR_ARRAY(edgeVertsNeighbor , i) { 
                        auto vIdx = edgeVertsNeighbor[i];
                        auto& pos = neighborQuadrant.verts[vIdx].pos;
                        pos = averagePos;
                    }
                }
            }

            if (hasNeighborLeft)
            {
                auto& quadrant = quadrants[z][x]; //current active one
                auto& neighborQuadrant = quadrants[z][x-1];
                const auto quadrantIdxNeighbor = GetQuadrantIndex(z, x-1);
                editing.dirtyQuadrants.Append(quadrantIdxNeighbor);

                for(idx_t z = 0; z < quadrant.CORNER_COUNT; ++z){
                    auto& edgeVerts         = quadrant.corners[z][0]; //left edge
                    auto& edgeVertsNeighbor = neighborQuadrant.corners[z][quadrant.CORNER_COUNT - 1]; //right edge

                    const auto averagePos = [&]
                    { 
                        auto  vIdx         = edgeVerts[0];
                        auto  vIdxNeighbor = edgeVertsNeighbor[0];
                        auto& pos          = quadrant.verts[vIdx].pos;
                        auto& posNeighbor  = neighborQuadrant.verts[vIdxNeighbor].pos;

                        return (pos + posNeighbor) * 0.5f;  
                    }();

                    FOR_ARRAY(edgeVerts , i) { 
                        auto vIdx = edgeVerts[i];
                        auto& pos = quadrant.verts[vIdx].pos;
                        pos = averagePos;
                    }

                    FOR_ARRAY(edgeVertsNeighbor , i) { 
                        auto vIdx = edgeVertsNeighbor[i];
                        auto& pos = neighborQuadrant.verts[vIdx].pos;
                        pos = averagePos;
                    }
                }
            }

            if (hasNeighbofTop)
            {
                auto& quadrant = quadrants[z][x]; //current active one
                auto& neighborQuadrant = quadrants[z-1][x];
                const auto quadrantIdxNeighbor = GetQuadrantIndex(z-1, x);
                editing.dirtyQuadrants.Append(quadrantIdxNeighbor);

                for(idx_t x = 0; x < quadrant.CORNER_COUNT; ++x){
                    auto& edgeVerts         = quadrant.corners[0][x]; //top edge
                    auto& edgeVertsNeighbor = neighborQuadrant.corners[quadrant.CORNER_COUNT - 1][x]; //bottom edge

                    const auto averagePos = [&]
                    { 
                        auto  vIdx         = edgeVerts[0];
                        auto  vIdxNeighbor = edgeVertsNeighbor[0];
                        auto& pos          = quadrant.verts[vIdx].pos;
                        auto& posNeighbor  = neighborQuadrant.verts[vIdxNeighbor].pos;

                        return (pos + posNeighbor) * 0.5f;  
                    }();

                    FOR_ARRAY(edgeVerts , i) { 
                        auto vIdx = edgeVerts[i];
                        auto& pos = quadrant.verts[vIdx].pos;
                        pos = averagePos;
                    }

                    FOR_ARRAY(edgeVertsNeighbor , i) { 
                        auto vIdx = edgeVertsNeighbor[i];
                        auto& pos = neighborQuadrant.verts[vIdx].pos;
                        pos = averagePos;
                    }
                }
            }

            //TODO: diagonal case
            //TODO: recalculate normals extra function, needs to be called after stiching
        }

        void Save(chars_t path = "res/terrain.txt")
        {
            using namespace math;
            LOG("saving terrain");

            for(idx_t z = 0; z < QUADRANT_COUNT; ++z) {
            for(idx_t x = 0; x < QUADRANT_COUNT; ++x) {

                char path2[] { "res/xxx" };
                path2[4] = 't';
                path2[5] = (char)(48 + z);
                path2[6] = (char)(48 + x);
                
                std::ofstream file(path2, std::ios::binary);
                if (!file) ERR("cannot open file");

                const auto& quadrant = quadrants[z][x];
                file.write((const char*)quadrant.verts, sizeof(utils::Common_Vertex) * quadrant.VERT_COUNT_TOTAL);
            }}
        }

        void Load(chars_t path = "res/terrain.txt")
        {
            using namespace math;
            LOG("loading terrain");

            for(idx_t z = 0; z < QUADRANT_COUNT; ++z) {
            for(idx_t x = 0; x < QUADRANT_COUNT; ++x) {

                char path2[] { "res/xxx" };
                path2[4] = 't';
                path2[5] = (char)(48 + z);
                path2[6] = (char)(48 + x);
                 
                std::ifstream file(path2, std::ios::binary);
                if (!file) ERR("cannot open file");

                const auto& quadrant = quadrants[z][x];
                file.read((char*)quadrant.verts, sizeof(utils::Common_Vertex) * quadrant.VERT_COUNT_TOTAL);

                editing.dirtyQuadrants.Append(GetQuadrantIndex(z, x));
            }}

        }
    };

}//ns