//https://github.com/awwdev

#pragma once

#include "com/box/Array.hpp"
#include "ecs/ECS.hpp"
#include "gpu/Meta/Cameras.hpp"
#include "res/Terrain/TerrainQuadrant.hpp"
#include "res/Terrain/TerrainSerialization.hpp"
#include "ecs/Prefabs/_Old/PrefabTypes.hpp"
#include "com/Algorithms.hpp"

#include <cstdlib> //rand

namespace rpg::res {

enum EditMode { VertexGrab, VertexPaint, PropPlacement };

//used by UI to set stuff
template<auto VERT_COUNT_TOTAL, auto QUADRANT_COUNT_TOTAL>
struct Settings
{
    EditMode mode = VertexGrab;
    com::Optional<com::Intersection> intersection {};

    struct VertexBrushInfo { idx_t idx; f32 falloff; };
    com::Array<VertexBrushInfo, VERT_COUNT_TOTAL> editingVertIndices;

    com::Array<idx_t, QUADRANT_COUNT_TOTAL> dirtyQuadrants;
    bool  isDragging  = false;
    f32   yGrabRef    = 0;
    f32   dragScale   = 0.3f;
    idx_t quadrantIdx = 0;
    f32   brushSize   = 1;

    com::Vec4f vertexColor { 1, 1, 1, 1 };

    ecs::ID gizmoID;
    com::Vec3f intersectionPos;

    ecs::PrefabType prefabType; 
    bool baked = false;
};


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
    using SETTINGS_T = Settings<QUADRANT_T::VERT_COUNT_TOTAL, QUADRANT_COUNT_TOTAL>;

    QUADRANT_T quadrants [QUADRANT_COUNT][QUADRANT_COUNT];
    //TODO make one vertex buffer and quadrants as views
    SETTINGS_T settings {};

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

    void Update(const double, const gpu::EgoCamera& camera, ecs::ECS& ecs)
    {   
        //? META
        if (app::glo::inputMode != app::glo::Edit_Mode)
            return;

        UpdateGizmos(ecs);

        settings.dirtyQuadrants.Clear();

        //? INTERACTION
        if (settings.mode == EditMode::VertexGrab)
            Grabbing(camera);
        if (settings.mode == EditMode::VertexPaint)
            Painting(camera);
        if (settings.mode == EditMode::PropPlacement)
            Placing(camera, ecs);

        //? MODES
        if (wnd::HasEvent<wnd::EventType::F5, wnd::EventState::Pressed>())
            SaveTerrain(quadrants);
        if (wnd::HasEvent<wnd::EventType::F6, wnd::EventState::Pressed>()){
            LoadTerrain(quadrants);
            MarkAllDirty();
        }
        if (wnd::HasEvent<wnd::EventType::F7, wnd::EventState::Pressed>())
            Stiching();
         if (wnd::HasEvent<wnd::EventType::F8, wnd::EventState::Pressed>())
            Bake();

        if (wnd::HasEvent<wnd::EventType::F2, wnd::EventState::Pressed>())
            settings.mode = (settings.mode == EditMode::VertexGrab) ? EditMode::VertexPaint : EditMode::VertexGrab;
        if (wnd::HasEvent<wnd::EventType::F3, wnd::EventState::Pressed>())
            settings.mode = EditMode::PropPlacement;

        if (wnd::HasEvent<wnd::EventType::N0, wnd::EventState::Pressed>()) settings.quadrantIdx = 0;
        if (wnd::HasEvent<wnd::EventType::N1, wnd::EventState::Pressed>()) settings.quadrantIdx = 1;
        if (wnd::HasEvent<wnd::EventType::N2, wnd::EventState::Pressed>()) settings.quadrantIdx = 2;
        if (wnd::HasEvent<wnd::EventType::N3, wnd::EventState::Pressed>()) settings.quadrantIdx = 3;
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

    void MarkAllDirty(){ //could be done better 
        settings.dirtyQuadrants.Clear();
        for(idx_t z = 0; z < QUADRANT_COUNT; ++z) {
        for(idx_t x = 0; x < QUADRANT_COUNT; ++x) {
            settings.dirtyQuadrants.Append(GetQuadrantIndex(z, x));
        }}
    }

    void InitGizmos(ecs::ECS& ecs)
    {
        settings.gizmoID = ecs.AddEntity();
        //ecs.arrays.AddComponent<ecs::ComponentEnum::RenderComponent>(settings.gizmoID, res::ModelType::PrimitiveRing16);
        //ecs.arrays.AddComponent<ecs::ComponentEnum::TransformComponent> (settings.gizmoID);
    }

    void UpdateGizmos(ecs::ECS& ecs)
    {
        using namespace com;
        auto& transform = ecs.arrays.transforms.Get(settings.gizmoID);
        const auto S = settings.brushSize;
        transform.scale = { S, S, S };
        transform.translation = settings.intersectionPos;
        transform.translation.y -= 0.1f; //z-fighting
    }

    //? INTERACTION

    com::Optional<com::Intersection> CheckIntersection(const gpu::EgoCamera& camera)
    {
        auto& quadrant = GetQuadrant(settings.quadrantIdx);
        const auto ray = ScreenRay(camera);

        for(idx_t i = 0; i < quadrant.VERT_COUNT_TOTAL; i+=3)
        {
            auto& v0 = quadrant.verts[i+0].pos;
            auto& v1 = quadrant.verts[i+1].pos;
            auto& v2 = quadrant.verts[i+2].pos;

            if (const auto intersection = com::RayTriangleIntersection(camera.position, ray, v0, v1, v2))
                return intersection;
        }

        return {};
    }

    void CollectVertsInBrushCircle()
    {
        auto& quadrant = GetQuadrant(settings.quadrantIdx);
        settings.editingVertIndices.Clear();

        FOR_CARRAY(quadrant.verts, i){
            const auto& vec1 = settings.intersectionPos;
            const auto& vec2 = quadrant.verts[i].pos;
            const auto  dist = com::Distance(vec2, vec1);

            if(dist < settings.brushSize)
                settings.editingVertIndices.Append(i, 1 - com::Ease(dist/settings.brushSize));
        }
    }

    void Painting(const gpu::EgoCamera& camera)
    {
        using namespace com;
        auto& quadrant = GetQuadrant(settings.quadrantIdx);

        if (const auto intersection = CheckIntersection(camera))
        {
            settings.intersectionPos = intersection->pos;
            if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Held>())
            {
                CollectVertsInBrushCircle(); //TODO: do at lower frequency

                FOR_ARRAY(settings.editingVertIndices, i){
                    const auto idx     = settings.editingVertIndices[i].idx;
                    //const auto falloff = settings.editingVertIndices[i].falloff;
                    quadrant.verts[idx].col.x = settings.vertexColor.x;
                    quadrant.verts[idx].col.y = settings.vertexColor.y;
                    quadrant.verts[idx].col.z = settings.vertexColor.z;
                }

                settings.dirtyQuadrants.Append(settings.quadrantIdx);
            }
        }
    }

    void Grabbing(const gpu::EgoCamera& camera)
    {
        using namespace com;
        auto& quadrant = GetQuadrant(settings.quadrantIdx);

        if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Released>())
            settings.intersection = {};

        if (const auto intersection = CheckIntersection(camera))
        {
            if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Pressed>()){
                settings.yGrabRef = (f32)wnd::glo::mouse_wy;
                settings.intersection = intersection;
                CollectVertsInBrushCircle();
            }
            if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Held>() == false)
                settings.intersectionPos = intersection->pos;
        }

        if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Held>() && settings.intersection){
            const f32 yDelta = wnd::glo::mouse_wy - settings.yGrabRef;
            settings.yGrabRef = (f32)wnd::glo::mouse_wy;
        
            FOR_ARRAY(settings.editingVertIndices, i){
                const auto idx     = settings.editingVertIndices[i].idx;
                const auto falloff = settings.editingVertIndices[i].falloff;
                quadrant.verts[idx].pos.y += yDelta * settings.dragScale * falloff;

                const auto triangleIdx = (idx / 3) * 3;
                quadrant.RecalculateNormalsOfTriangle(triangleIdx);
            }

            settings.intersectionPos.y += yDelta * settings.dragScale; //TODO: would need falloff of closest intersection vertex
            settings.dirtyQuadrants.Append(settings.quadrantIdx);
        }        
    }

    //TODO: we don't acutally place in terrain, but use terrain to cast ray against
    void Placing(const gpu::EgoCamera& camera, ecs::ECS& ecs)
    {
        using namespace com;
        //auto& quadrant = GetQuadrant(settings.quadrantIdx);

        if (const auto intersection = CheckIntersection(camera))
        {
            settings.intersectionPos = intersection->pos;
            if (wnd::HasEvent<wnd::EventType::Mouse_ButtonLeft, wnd::EventState::Pressed>()) {

                for(auto i = 0; i < 10; ++i) //for grass test 
                {
                    const auto ID = ecs.AddEntity(settings.prefabType);
                    auto& t = ecs.arrays.transforms.Get(ID);

                    Vec3f pos = intersection->pos;
                    pos.x += ((std::rand() % 40) / 40.f) * 7;
                    pos.z += ((std::rand() % 40) / 40.f) * 7;

                    //t.translation = intersection->pos;
                    t.translation = pos;
                    const f32 randY = (f32) (std::rand() % 360);
                    t.rotation = { 0, randY, 0 };
                    const f32 SX = 0.8f + (std::rand() % 40) / 40.f;
                    const f32 SZ = 0.8f + (std::rand() % 40) / 40.f;
                    const f32 SY = 0.8f + (std::rand() % 40) / 40.f;
                    t.scale = { SX, SY, SZ };
                }

                
            }
        }
    }

    //? STICHING

    void StichCorner(const idx_t qcz, const idx_t qcx, const idx_t cornerCount) 
    {
        com::Array<com::Vec3f, 4> positions;
        com::Array<gpu::RenderData_General::Vertex*, 6> verts;

        //TL
        if (qcx > 0 && qcz > 0){
            auto& quadrant = quadrants[qcz - 1][qcx - 1];
            const auto vertIndices = quadrant.GetVerticesByCorner({quadrant.CORNER_COUNT - 1, quadrant.CORNER_COUNT - 1});
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
            com::Vec3f pos {};
            FOR_ARRAY(positions, i) pos = pos + positions[i];
            pos = pos * (1/(float)positions.Count());
            return pos;
        }();

        FOR_ARRAY(verts, i){
            verts[i]->pos = avgPos;
        }
    }

    void Bake()
    {
        settings.baked = !settings.baked;
    }

    void Stiching()
    {
        dbg::LogInfo("stiching terrain");

        //current coord of active editing quadrant
        const auto z = settings.quadrantIdx / QUADRANT_COUNT;
        const auto x = settings.quadrantIdx % QUADRANT_COUNT;
        settings.dirtyQuadrants.Append(settings.quadrantIdx); //!cleared by renderer
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
            const com::Array<idx_t, 6>& edgeVerts, 
            const com::Array<idx_t, 6>& edgeVertsNeighbor)
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
            settings.dirtyQuadrants.Append(quadrantIdxNeighbor);

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
            settings.dirtyQuadrants.Append(quadrantIdxNeighbor);

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
            settings.dirtyQuadrants.Append(quadrantIdxNeighbor);

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
            settings.dirtyQuadrants.Append(quadrantIdxNeighbor);

            for(idx_t x = 0; x < quadrant.CORNER_COUNT; ++x){
                auto& edgeVerts         = quadrant.corners[0][x]; //top edge
                auto& edgeVertsNeighbor = neighborQuadrant.corners[quadrant.CORNER_COUNT - 1][x]; //bottom edge
                stichFn(quadrant, neighborQuadrant, edgeVerts, edgeVertsNeighbor);    
            }
        }

        if (hasNeighborNE)
        {
            //auto& neighborQuadrant = quadrants[z-1][x+1];
            const auto quadrantIdxNeighbor = GetQuadrantIndex(z-1, x+1);
            settings.dirtyQuadrants.Append(quadrantIdxNeighbor);
            StichCorner(z-1, x+1, quadrant.CORNER_COUNT);
        }

        if (hasNeighborNW)
        {
            //auto& neighborQuadrant = quadrants[z-1][x-1];
            const auto quadrantIdxNeighbor = GetQuadrantIndex(z-1, x-1);
            settings.dirtyQuadrants.Append(quadrantIdxNeighbor);
            StichCorner(z-1, x-1, quadrant.CORNER_COUNT);
        }

        if (hasNeighborSE)
        {
            //auto& neighborQuadrant = quadrants[z+1][x+1];
            const auto quadrantIdxNeighbor = GetQuadrantIndex(z+1, x+1);
            settings.dirtyQuadrants.Append(quadrantIdxNeighbor);
            StichCorner(z+1, x+1, quadrant.CORNER_COUNT);
        }

        if (hasNeighborSW)
        {
            //auto& neighborQuadrant = quadrants[z+1][x-1];
            const auto quadrantIdxNeighbor = GetQuadrantIndex(z+1, x-1);
            settings.dirtyQuadrants.Append(quadrantIdxNeighbor);
            StichCorner(z+1, x-1, quadrant.CORNER_COUNT);
        }

        //recalc normals
        FOR_ARRAY(settings.dirtyQuadrants, i)
        {
            const auto quadrantIdx = settings.dirtyQuadrants[i];
            GetQuadrant(quadrantIdx).RecalculateNormals();
        }

    }

};

}//ns    