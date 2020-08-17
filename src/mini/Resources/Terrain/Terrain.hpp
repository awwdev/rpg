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
        //? META
        if (app::global::inputMode != app::global::Edit_Mode)
            return;

        UpdateGizmos(ecs);

        editing.dirtyQuadrants.Clear();

        //? INTERACTION
        if (editing.mode == Editing::EditMode::VertexGrab)
            Grabbing(camera);
        if (editing.mode == Editing::EditMode::VertexPaint)
            Painting(camera);

        //? MODES
        if (wnd::HasEvent<wnd::F5, wnd::Pressed>())
            SaveTerrain(quadrants);
        if (wnd::HasEvent<wnd::F6, wnd::Pressed>()){
            LoadTerrain(quadrants);
            MarkAllDirty();
        }
        if (wnd::HasEvent<wnd::F7, wnd::Pressed>())
            Stiching();
        if (wnd::HasEvent<wnd::F2, wnd::Pressed>())
            editing.mode = (editing.mode == Editing::VertexGrab) ? Editing::VertexPaint : Editing::VertexGrab;

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

    struct Editing
    {
        enum EditMode { VertexGrab, VertexPaint } mode = VertexGrab;
        box::Optional<utils::Intersection> intersection {};

        struct VertexBrushInfo { idx_t idx; f32 falloff; };
        box::Array<VertexBrushInfo, QUADRANT_T::VERT_COUNT_TOTAL> editingVertIndices;

        box::Array<idx_t, QUADRANT_COUNT_TOTAL> dirtyQuadrants;
        bool isDragging  = false;
        f32  yGrabRef  = 0;
        f32  dragScale   = 0.05f;
        u32  quadrantIdx = 0;
        f32  brushSize   = 1;

        utils::Vec4f vertexColor { 1, 1, 1, 1 };

        ecs::ID gizmoID;
        utils::Vec3f intersectionPos;
    } editing;

    void MarkAllDirty(){ //could be done better 
        editing.dirtyQuadrants.Clear();
        for(idx_t z = 0; z < QUADRANT_COUNT; ++z) {
        for(idx_t x = 0; x < QUADRANT_COUNT; ++x) {
            editing.dirtyQuadrants.Append(GetQuadrantIndex(z, x));
        }}
    }

    void InitGizmos(ecs::ECS& ecs)
    {
        editing.gizmoID = ecs.AddEntity();
        ecs.arrays.AddComponent<ecs::ComponentType::RenderData>(editing.gizmoID, res::MeshType::PrimitiveRing16);
        ecs.arrays.AddComponent<ecs::ComponentType::Transform> (editing.gizmoID, utils::Identity4());
    }

    void UpdateGizmos(ecs::ECS& ecs)
    {
        using namespace utils;
        auto& transform = ecs.arrays.transforms.Get(editing.gizmoID);
        const auto S = editing.brushSize;
        transform.transform = {
            S, 0, 0, 0,
            0, S, 0, 0,
            0, 0, S, 0,
            0, 0, 0, 1,
        };
        transform.transform[3][0] = editing.intersectionPos[X];
        transform.transform[3][1] = editing.intersectionPos[Y] - 0.1f; //z fighting
        transform.transform[3][2] = editing.intersectionPos[Z];
    }

    //? INTERACTION

    box::Optional<utils::Intersection> CheckIntersection(const rendering::EgoCamera& camera)
    {
        auto& quadrant = GetQuadrant(editing.quadrantIdx);
        const auto ray = ScreenRay(camera);

        for(idx_t i = 0; i < quadrant.VERT_COUNT_TOTAL; i+=3)
        {
            auto& v0 = quadrant.verts[i+0].pos;
            auto& v1 = quadrant.verts[i+1].pos;
            auto& v2 = quadrant.verts[i+2].pos;

            if (const auto intersection = utils::RayTriangleIntersection(camera.position, ray, v0, v1, v2))
                return intersection;
        }

        return {};
    }

    void CollectVertsInBrushCircle()
    {
        auto& quadrant = GetQuadrant(editing.quadrantIdx);
        editing.editingVertIndices.Clear();

        FOR_CARRAY(quadrant.verts, i){
            const auto& vec1 = editing.intersectionPos;
            const auto& vec2 = quadrant.verts[i].pos;
            const auto  dist = utils::Distance(vec2, vec1);

            if(dist < editing.brushSize)
                editing.editingVertIndices.Append(i, 1 - utils::Ease(dist/editing.brushSize));
        }
    }

    void Painting(const rendering::EgoCamera& camera)
    {
        using namespace utils;
        auto& quadrant = GetQuadrant(editing.quadrantIdx);

        if (const auto intersection = CheckIntersection(camera))
        {
            editing.intersectionPos = intersection->pos;
            if (wnd::HasEvent<wnd::Mouse_ButtonLeft, wnd::Held>())
            {
                CollectVertsInBrushCircle(); //TODO: do at lower frequency

                FOR_ARRAY(editing.editingVertIndices, i){
                    const auto idx     = editing.editingVertIndices[i].idx;
                    const auto falloff = editing.editingVertIndices[i].falloff;
                    quadrant.verts[idx].col[X] = editing.vertexColor[X];
                    quadrant.verts[idx].col[Y] = editing.vertexColor[Y];
                    quadrant.verts[idx].col[Z] = editing.vertexColor[Z];
                }

                editing.dirtyQuadrants.Append(editing.quadrantIdx);
            }
        }
    }

    void Grabbing(const rendering::EgoCamera& camera)
    {
        using namespace utils;
        auto& quadrant = GetQuadrant(editing.quadrantIdx);

        if (wnd::HasEvent<wnd::Mouse_ButtonLeft, wnd::Released>())
            editing.intersection = {};

        if (const auto intersection = CheckIntersection(camera))
        {
            if (wnd::HasEvent<wnd::Mouse_ButtonLeft, wnd::Pressed>()){
                editing.yGrabRef = (f32)wnd::global::mouse_wy;
                editing.intersection = intersection;
                CollectVertsInBrushCircle();
            }
            if (wnd::HasEvent<wnd::Mouse_ButtonLeft, wnd::Held>() == false)
                editing.intersectionPos = intersection->pos;
        }

        if (wnd::HasEvent<wnd::Mouse_ButtonLeft, wnd::Held>() && editing.intersection){
            const f32 yDelta = wnd::global::mouse_wy - editing.yGrabRef;
            editing.yGrabRef = (f32)wnd::global::mouse_wy;
        
            FOR_ARRAY(editing.editingVertIndices, i){
                const auto idx     = editing.editingVertIndices[i].idx;
                const auto falloff = editing.editingVertIndices[i].falloff;
                quadrant.verts[idx].pos[Y] += yDelta * editing.dragScale * falloff;

                const auto triangleIdx = (idx / 3) * 3;
                quadrant.RecalculateNormalsOfTriangle(triangleIdx);
            }

            editing.intersectionPos[Y] += yDelta * editing.dragScale; //TODO: would need falloff of closest intersection vertex
            editing.dirtyQuadrants.Append(editing.quadrantIdx);
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