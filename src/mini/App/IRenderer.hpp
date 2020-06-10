//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Box/AlignedStorage.hpp"
#include "mini/Utils/Vertex.hpp"
#include "mini/Resources/FontMap.hpp"
#include "mini/Resources/PrimitiveMeshes.hpp"
#undef DrawText

namespace mini::app
{
    struct VertexGroup
    {
        u32 v1, v2; //first and last
        u32 i1, i2;

        inline u32 VertexCount() const { return (v2 - v1) + 1; }
        inline u32 IndexCount()  const { return (i2 - i1) + 1; }
    };

    //TODO: maybe in future pass something like a RenderCollector to Scene instead of whole Renderer? (RenderGraph)
    struct IRenderer
    {
        inline void FrameBegin()
        {
            //? for dynamic buffers
            vertices.Clear();
            indices.Clear();
            uniforms.Clear();
            vertexGroups.Clear();
        }


        inline void Add_DrawQuad(const Rect<int>& rect, math::Vec4f col = { 1, 1, 1, 1})
        {
            auto& group = vertexGroups.AppendReturn();
            group.v1 = vertices.Count();
            group.i1 = indices.Count();

            const auto quad = res::CreateRect<res::Indexed::Yes>(rect, { 0, 0, 0, 0 }, col);
            const u32 c = vertices.Count();
            vertices.AppendArray(quad.verts);
            uint32_t idxs [] {c + 0u, c + 2u, c + 3u, c + 0u, c + 1u, c + 2u};
            indices.AppendArray(idxs);
            uniforms.Append({false});

            group.v2 = vertices.Count() - 1;
            group.i2 = indices.Count()  - 1;
        }


        inline void Add_DrawText(chars_t text, const int x, int y)
        {
            auto& group = vertexGroups.AppendReturn();
            group.v1 = vertices.Count();
            group.i1 = indices.Count();

            //TODO: get the font data from some resource (struct Font) 
            const auto fw = 14; //real letter tile size in px
            const auto fh = 18; 
            const auto s  = 1; //scale

            auto xx = x; 
            for(auto i=0; text[i] != '\0'; ++i)
            {
                xx += fw * s + 0; //padding
                if (text[i] == '\n') 
                {
                    y += fh + 4; //+padding
                    xx = x;
                    continue;
                }

                const auto& coords = res::MAPPING.GetValue(text[i]);
                const auto quad = res::CreateRect<res::Indexed::Yes>(
                    Rect<int>{ xx, y, fw * s, fh * s}, 
                    Rect<int>{ coords[Vx] * fw, coords[Vy] * fh, fw, fh },
                    { 0, 0, 0, 1}
                );

                const u32 c = vertices.Count();
                vertices.AppendArray(quad.verts);
                uint32_t idxs[] {c + 0, c + 1, c + 2, c + 2, c + 3, c + 0};
                indices.AppendArray(idxs);
            }

            group.v2 = vertices.Count() - 1;
            group.i2 = indices.Count()  - 1;
            uniforms.Append({true});
        }


        inline void Add_DrawButton(
            chars_t text, 
            const Rect<int>& rect, 
            math::Vec4f col = { 1, 1, 1, 1})
        {
            Add_DrawQuad(rect, col);
            //TODO: find center for text
            Add_DrawText(text, rect.x, rect.y);
        }



        //? whole renderer is on heap
        //? gpu api agnostic resources (that will be used in derived class to upload to gpu)
        box::Array<Vertex, 2000> vertices; 
        box::Array<uint32_t, 3000> indices;
        box::AlignedStorage<2000, vk::UboData_Default> uniforms;
        
        box::Array<VertexGroup, 500> vertexGroups; //outlines vertex and index array (so we know when to change state)
        
        
    };

}//ns