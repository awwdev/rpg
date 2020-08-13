//https://github.com/awwdev

#pragma once
#include "mini/Utils/Structs.hpp"
#include "mini/Box/Array.hpp"
#include "mini/ECS/ECS.hpp"
#include "mini/Rendering/Cameras.hpp"
#include "mini/Resources/Terrain/TerrainQuadrant.hpp"
#include "mini/Resources/Terrain/TerrainSerialization.hpp"

namespace mini::res {

template<
    auto QUAD_COUNT_T, 
    auto QUAD_LEN_T, 
    auto QUADRANT_COUNT_T>
struct Terrain
{
    static constexpr idx_t QUAD_COUNT = (idx_t)QUAD_COUNT_T;
    static constexpr idx_t QUAD_LEN   = (idx_t)QUAD_LEN_T;

    static constexpr idx_t QUADRANT_COUNT       = (idx_t)QUADRANT_COUNT_T;
    static constexpr idx_t QUADRANT_LEN         = QUAD_COUNT * QUAD_LEN;
    static constexpr idx_t QUADRANT_COUNT_TOTAL = QUADRANT_COUNT * QUADRANT_COUNT;
    static constexpr idx_t QUADRANT_LEN_TOTAL   = QUADRANT_LEN * QUADRANT_COUNT;

    using QUADRANT_T = Quadrant<QUAD_COUNT, QUADRANT_LEN>;
    QUADRANT_T quadrants [QUADRANT_COUNT][QUADRANT_COUNT];

    enum EditMode { VertexGrab, VertexPaint } mode = VertexGrab;

    //? MAIN

    void Create()
    {
        for(idx_t z = 0; z < QUADRANT_COUNT; ++z) {
        for(idx_t x = 0; x < QUADRANT_COUNT; ++x) {
            quadrants[z][x].Create(
                (f32)z * QUADRANT_LEN - QUADRANT_LEN_TOTAL * 0.5f, 
                (f32)x * QUADRANT_LEN - QUADRANT_LEN_TOTAL * 0.5f);
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
            SaveTerrain(quadrants);
        if (wnd::HasEvent<wnd::F6, wnd::Pressed>()){
            LoadTerrain(quadrants);
            MarkAllDirty();
        }
        if (wnd::HasEvent<wnd::F7, wnd::Pressed>())
            Stiching();

        if (wnd::HasEvent<wnd::N0, wnd::Pressed>()) editing.quadrantIdx = 0;
        if (wnd::HasEvent<wnd::N1, wnd::Pressed>()) editing.quadrantIdx = 1;
        if (wnd::HasEvent<wnd::N2, wnd::Pressed>()) editing.quadrantIdx = 2;
        if (wnd::HasEvent<wnd::N3, wnd::Pressed>()) editing.quadrantIdx = 3;
    }

    //? HELPER

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

    //? EDITING

    struct 
    {
        box::Array<idx_t, 6> draggingVertIndices;
        box::Array<idx_t, QUADRANT_COUNT_TOTAL> dirtyQuadrants;
        bool isDragging  = false;
        f32  yDragPoint  = 0;
        f32  dragScale   = 0.05f;
        u32  quadrantIdx = 0;
    } editing;

    void MarkAllDirty(){ //could be done better 
        editing.dirtyQuadrants.Clear();
        for(idx_t z = 0; z < QUADRANT_COUNT; ++z) {
        for(idx_t x = 0; x < QUADRANT_COUNT; ++x) {
            editing.dirtyQuadrants.Append(GetQuadrantIndex(z, x));
        }}
    }

    //? INTERACTION

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

                if (mode == VertexGrab)
                {
                    if (wnd::HasEvent<wnd::Mouse_ButtonLeft, wnd::Pressed>()){
                        editing.draggingVertIndices.Clear();
                        const auto corner   = quadrant.GetCornerByVertex(closestVertex);
                        const auto vertices = quadrant.GetVerticesByCorner(corner);
                        editing.draggingVertIndices.AppendArray(vertices);
                        editing.yDragPoint = (f32)wnd::global::mouse_wy;
                        editing.isDragging = true; 
                    }
                }

                if (mode == VertexPaint)
                {
                    if (wnd::HasEvent<wnd::Mouse_ButtonLeft, wnd::PressedOrHeld>()){

                    }
                }                
            }
        }
    }

    void Dragging()
    {
        using namespace utils;
        auto& quadrant = GetQuadrant(editing.quadrantIdx);
        editing.dirtyQuadrants.Append(editing.quadrantIdx);

        const f32 yDelta = wnd::global::mouse_wy - editing.yDragPoint;
        editing.yDragPoint = (f32)wnd::global::mouse_wy;
        
        const auto& vertIndices = editing.draggingVertIndices;
        FOR_ARRAY(vertIndices, i){
            const auto idx = vertIndices[i];
            quadrant.verts[idx].pos[Y] += yDelta * editing.dragScale;

            const auto triangleIdx = (idx / 3) * 3;
            quadrant.RecalculateNormalsOfTriangle(triangleIdx);
        }
    }


    //? STICHING

    void StichCorner(const idx_t qcz, const idx_t qcx, const idx_t cornerCount) 
    {
        box::Array<utils::Vec3f, 4> positions;
        box::Array<utils::Common_Vertex*, 6> verts;

        //TL
        if (qcx > 0 && qcz > 0){
            auto& quadrant = quadrants[qcz - 1][qcx - 1];
            const auto  vertIndices = quadrant.GetVerticesByCorner({quadrant.CORNER_COUNT - 1, quadrant.CORNER_COUNT - 1});
            FOR_ARRAY(vertIndices, i){
                verts.Append(&quadrant.verts[vertIndices[i]]);
            }
            positions.Append(quadrant.verts[vertIndices[0]].pos);
        }

        //BL
        if (qcx > 0 && qcz < QUADRANT_COUNT + 1){
            auto& quadrant = quadrants[qcz - 0][qcx - 1];
            const auto  vertIndices = quadrant.GetVerticesByCorner({0, quadrant.CORNER_COUNT - 1});
            FOR_ARRAY(vertIndices, i){
                verts.Append(&quadrant.verts[vertIndices[i]]);
            }
            positions.Append(quadrant.verts[vertIndices[0]].pos);
        }

        //TR
        if (qcx < QUADRANT_COUNT + 1 && qcz > 0){
            auto& quadrant = quadrants[qcz - 1][qcx - 0];
            const auto  vertIndices = quadrant.GetVerticesByCorner({quadrant.CORNER_COUNT - 1, 0});
            FOR_ARRAY(vertIndices, i){
                verts.Append(&quadrant.verts[vertIndices[i]]);
            }
            positions.Append(quadrant.verts[vertIndices[0]].pos);
        }

        //BR
        if (qcx < cornerCount && qcz < QUADRANT_COUNT + 1){
            auto& quadrant = quadrants[qcz - 0][qcx - 0];
            const auto  vertIndices = quadrant.GetVerticesByCorner({0, 0});
            FOR_ARRAY(vertIndices, i){
                verts.Append(&quadrant.verts[vertIndices[i]]);
            }
            positions.Append(quadrant.verts[vertIndices[0]].pos);
        }

        const auto avgPos = [&]{
            utils::Vec3f pos {};
            FOR_ARRAY(positions, i) pos = pos + positions[i];
            pos = pos * (1/(float)positions.count);
            return pos;
        }();

        FOR_ARRAY(verts, i){
            verts[i]->pos = avgPos;
        }
    }

    void Stiching()
    {
        dbg::LogInfo("stiching terrain");

        //current coord of active editing quadrant
        const auto z = editing.quadrantIdx / QUADRANT_COUNT;
        const auto x = editing.quadrantIdx % QUADRANT_COUNT;
        editing.dirtyQuadrants.Append(editing.quadrantIdx); //!cleared by renderer
        auto& quadrant = quadrants[z][x]; //current active one

        //neighbors
        const bool hasNeighborE = x < QUADRANT_COUNT - 1;
        const bool hasNeighborS = z < QUADRANT_COUNT - 1;
        const bool hasNeighborW = x > 0;
        const bool hasNeighborN = z > 0;

        const bool hasNeighborNE = hasNeighborN && hasNeighborE;
        const bool hasNeighborNW = hasNeighborN && hasNeighborW;
        const bool hasNeighborSE = hasNeighborS && hasNeighborE;;
        const bool hasNeighborSW = hasNeighborS && hasNeighborW;;


        auto stichFn = [&](
            QUADRANT_T& quadrant,
            QUADRANT_T& neighborQuadrant,
            const box::Array<u32, 6>& edgeVerts, 
            const box::Array<u32, 6>& edgeVertsNeighbor)
        {
            const auto averagePos = [&]
            { 
                const auto  vIdx         = edgeVerts[0];
                const auto  vIdxNeighbor = edgeVertsNeighbor[0];
                const auto& pos          = quadrant.verts[vIdx].pos;
                const auto& posNeighbor  = neighborQuadrant.verts[vIdxNeighbor].pos;

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
        };

        //TODO: spare the very corner
        if (hasNeighborE)
        {
            auto& neighborQuadrant = quadrants[z][x+1];
            const auto quadrantIdxNeighbor = GetQuadrantIndex(z, x+1);
            editing.dirtyQuadrants.Append(quadrantIdxNeighbor);

            for(idx_t z = 0; z < quadrant.CORNER_COUNT; ++z){
                auto& edgeVerts         = quadrant.corners[z][quadrant.CORNER_COUNT - 1]; //right edge
                auto& edgeVertsNeighbor = neighborQuadrant.corners[z][0]; //left edge
                stichFn(quadrant, neighborQuadrant, edgeVerts, edgeVertsNeighbor);
            }
        }

        if (hasNeighborS)
        {
            auto& neighborQuadrant = quadrants[z+1][x];
            const auto quadrantIdxNeighbor = GetQuadrantIndex(z+1, x);
            editing.dirtyQuadrants.Append(quadrantIdxNeighbor);

            for(idx_t x = 0; x < quadrant.CORNER_COUNT; ++x){
                auto& edgeVerts         = quadrant.corners[quadrant.CORNER_COUNT - 1][x]; //bottom edge
                auto& edgeVertsNeighbor = neighborQuadrant.corners[0][x]; //top edge
                stichFn(quadrant, neighborQuadrant, edgeVerts, edgeVertsNeighbor);
            }
        }

        if (hasNeighborW)
        {
            auto& neighborQuadrant = quadrants[z][x-1];
            const auto quadrantIdxNeighbor = GetQuadrantIndex(z, x-1);
            editing.dirtyQuadrants.Append(quadrantIdxNeighbor);

            for(idx_t z = 0; z < quadrant.CORNER_COUNT; ++z){
                auto& edgeVerts         = quadrant.corners[z][0]; //left edge
                auto& edgeVertsNeighbor = neighborQuadrant.corners[z][quadrant.CORNER_COUNT - 1]; //right edge
                stichFn(quadrant, neighborQuadrant, edgeVerts, edgeVertsNeighbor);
            }
        }

        if (hasNeighborN)
        {
            auto& neighborQuadrant = quadrants[z-1][x];
            const auto quadrantIdxNeighbor = GetQuadrantIndex(z-1, x);
            editing.dirtyQuadrants.Append(quadrantIdxNeighbor);

            for(idx_t x = 0; x < quadrant.CORNER_COUNT; ++x){
                auto& edgeVerts         = quadrant.corners[0][x]; //top edge
                auto& edgeVertsNeighbor = neighborQuadrant.corners[quadrant.CORNER_COUNT - 1][x]; //bottom edge
                stichFn(quadrant, neighborQuadrant, edgeVerts, edgeVertsNeighbor);    
            }
        }

        if (hasNeighborNE)
        {
            auto& neighborQuadrant = quadrants[z-1][x+1];
            const auto quadrantIdxNeighbor = GetQuadrantIndex(z-1, x+1);
            editing.dirtyQuadrants.Append(quadrantIdxNeighbor);
            StichCorner(z-1, x+1, quadrant.CORNER_COUNT);
        }

        if (hasNeighborNW)
        {
            auto& neighborQuadrant = quadrants[z-1][x-1];
            const auto quadrantIdxNeighbor = GetQuadrantIndex(z-1, x-1);
            editing.dirtyQuadrants.Append(quadrantIdxNeighbor);
            StichCorner(z-1, x-1, quadrant.CORNER_COUNT);
        }

        if (hasNeighborSE)
        {
            auto& neighborQuadrant = quadrants[z+1][x+1];
            const auto quadrantIdxNeighbor = GetQuadrantIndex(z+1, x+1);
            editing.dirtyQuadrants.Append(quadrantIdxNeighbor);
            StichCorner(z+1, x+1, quadrant.CORNER_COUNT);
        }

        if (hasNeighborSW)
        {
            auto& neighborQuadrant = quadrants[z+1][x-1];
            const auto quadrantIdxNeighbor = GetQuadrantIndex(z+1, x-1);
            editing.dirtyQuadrants.Append(quadrantIdxNeighbor);
            StichCorner(z+1, x-1, quadrant.CORNER_COUNT);
        }

        //recalc normals
        FOR_ARRAY(editing.dirtyQuadrants, i)
        {
            const auto quadrantIdx = editing.dirtyQuadrants[i];
            GetQuadrant(quadrantIdx).RecalculateNormals();
        }

    }

};

}//ns    