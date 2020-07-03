//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Box/AlignedStorage.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/RenderGraph/UboData.hpp"
#include "mini/Resources/PrimitiveMeshes.hpp"
#include "mini/Resources/HostResources.hpp"

//! not used right now, use as ref for vbo based rendering

namespace mini::rendergraph
{

    /*
    using RenderID = u32;

    struct RenderGroup_UI 
    {
        u32 v1, v2;
        u32 i1, i2;
    };

    struct RenderArray_UI
    {
        box::Array<RenderGroup_UI, 100>      groups;   //draw entity and ubo are "vertical" 
        box::AlignedStorage<UboData_UI, 100> uniforms;
        box::Array<utils::VertexUI, 1000>    vertices;
        box::Array<uint32_t, 1500>           indices;

        RenderGroup_UI& BeginGroup()
        {
            RenderGroup_UI& group = groups.AppendReturn();
            group.v1 = vertices.Count();
            group.i1 = indices.Count();
            return group;
        }

        void EndGroup(RenderGroup_UI& group)
        {
            group.v2 = vertices.Count() - 1;
            group.i2 = indices.Count()  - 1;
        }
    };


    struct RenderGraph
    {
        RenderArray_UI renderArray_UI;

        void Add_UI(
            const box::IArray<utils::VertexUI>& vertices,
            const box::IArray<uint32_t>& indices,
            const UboData_UI& uniform)
        {
            auto& group = renderArray_UI.BeginGroup();
            renderArray_UI.vertices.AppendArray(vertices);
            renderArray_UI.indices.AppendArray(indices);
            renderArray_UI.uniforms.Append(uniform);
            renderArray_UI.EndGroup(group);
        }
    };

    struct VertexGroup
    {
        u32 v1, v2; //first and last
        u32 i1, i2;

        inline u32 VertexCount() const { return (v2 - v1) + 1; }
        inline u32 IndexCount()  const { return (i2 - i1) + 1; }
    };

    struct IRenderer
    {
        //?"CPU" resources that will be used to copy to GPU resources
        //complete clear of those (dynamic buffers)
        box::Array<utils::Vertex, 2000> vertices; 
        box::Array<uint32_t, 3000> indices;
        box::AlignedStorage<UboData_UI, 1000> uniforms; 
        box::Array<VertexGroup, 500> vertexGroups;
        box::Array<InstanceData_UI, 1000> instanceData; 

        //!hard dependency not needed 
        const hostRes::HostResources& hostResources;
        explicit IRenderer(const hostRes::HostResources& pHostResources) : hostResources {pHostResources} {;}


        void FrameBegin()
        {
            vertices.Clear();
            indices.Clear();
            uniforms.Clear();
            vertexGroups.Clear();
            instanceData.Clear();
        }

        void Add_DrawQuad(const utils::Rect<int>& pRect, const utils::RGBAColor4u& col = { 255, 255, 255, 255 })
        {
            auto& group = vertexGroups.AppendReturn();
            group.v1 = vertices.Count();
            group.i1 = indices.Count();

            const auto rect = mesh::CreateVertices_Rect<mesh::Indexed::Yes>(pRect, { 0, 0, 0, 0 }, utils::NormaliseColor(col));
            const u32 c = vertices.Count();
            vertices.AppendArray(rect.verts);
            uint32_t idxs [] {c + 0u, c + 2u, c + 3u, c + 0u, c + 1u, c + 2u};
            indices.AppendArray(idxs);
            uniforms.Append({false});

            //VBO has only "instance" data (offset, color)(UBO)
            //but no use of GPU instancing
            //no vertex buffer used
            //cmdDraw(totalVertCount)
            //inside vert shader
            //1) get "instance data" (int)(vertId / 4) and use it to construct vert 
            //2) construct the vert of a quad vertId % 4
            //no need for index buffer then
            

            instanceData.Append(InstanceData_UI{{ 0,  0}, {0, 1, 0, 1}});
            instanceData.Append(InstanceData_UI{{32, 32}, {0, 0, 1, 1}});
            instanceData.Append(InstanceData_UI{{64, 64}, {1, 0, 0, 1}});

            group.v2 = vertices.Count() - 1;
            group.i2 = indices.Count()  - 1;
        }

        //TODO: could do lots of customization
        void Add_DrawText(chars_t text, const int x, int y, const res::Font& font, const utils::RGBAColor4u& col = { 0, 0, 0, 255 })
        {
            auto& group = vertexGroups.AppendReturn();
            group.v1 = vertices.Count();
            group.i1 = indices.Count();

            const int fw = font.letter_w;
            const int fh = font.letter_h; 
            const auto s  = 0.5f; //scale

            auto xx = x; 
            for(auto i=0; text[i] != '\0'; ++i)
            {
                 if (text[i] == '\n') 
                {
                    y += fh + 4; //+padding
                    xx = x;
                    continue;
                }

                const auto& coords = font.fontMap.GetValue(text[i]);
                const auto quad = mesh::CreateVertices_Rect<mesh::Indexed::Yes>(
                    utils::Rect<int>{ xx, y, (int)(fw * s), (int)(fh * s)}, 
                    utils::Rect<int>{ coords[Vx] * fw, coords[Vy] * fh, fw, fh }
                );

                const u32 c = vertices.Count();
                vertices.AppendArray(quad.verts);
                uint32_t idxs[] {c + 0, c + 1, c + 2, c + 2, c + 3, c + 0};
                indices.AppendArray(idxs);
                xx += fw * s + 0; //padding
            }

            group.v2 = vertices.Count() - 1;
            group.i2 = indices.Count()  - 1;
            uniforms.Append({true});
        }


        void Add_DrawLabel(
            chars_t text, 
            const utils::Rect<int>& rect, 
            const utils::RGBAColor4u& quadCol = { 0, 0, 0, 1},
            const utils::RGBAColor4u& textCol = { 1, 1, 1, 1}
            )
        {
            Add_DrawQuad(rect, quadCol);
            const auto w = hostResources.fonts.default_font.letter_w * std::strlen(text);
            const auto h = hostResources.fonts.default_font.letter_h;
            const auto x = rect.x + (rect.w * 0.5f - w * 0.25f);
            const auto y = rect.y + (rect.h * 0.5f - h * 0.25f);
            Add_DrawText(text, x, y, hostResources.fonts.default_font, textCol);
        }

    };
    */

}//ns