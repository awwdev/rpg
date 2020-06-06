//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Utils/Vertex.hpp"
#include "mini/Resources/FontMap.hpp"
#include "mini/Resources/PrimitiveMeshes.hpp"
#undef DrawText

namespace mini
{
    struct VertexGroup
    {
        u32 v1, v2; //begin, end
        u32 i1, i2;

        inline u32 VertexCount() const { return (v2 - v1) + 1; }
        inline u32 IndexCount()  const { return (i2 - i1) + 1; }
    };

    
    struct IRenderer
    {
        inline void FrameBegin()
        {
            //for complete dynamic buffers
            vertices.Clear();
            indices.Clear();
            vertexGroups.Clear();
        }

        inline void DrawBox(s32 x, s32 y, s32 w, s32 h)
        {
            auto& meshView = vertexGroups.AppendReturn();
            meshView.v1 = vertices.Count();
            meshView.i1 = indices.Count();

            const auto quad = res::CreateRect<res::Indexed::Yes>({ x, y, w, h }, { 0, 0, 0, 0 }, { 0.f, 0.f, 1.f, 1.f });
            const u32 c = vertices.Count();
            uint32_t arr [] {c + 0u, c + 2u, c + 3u, c + 0u, c + 1u, c + 2u};
            vertices.AppendArray(quad.verts);
            indices.AppendArray(arr);

            meshView.v2 = vertices.Count() - 1;
            meshView.i2 = indices.Count()  - 1;
        }

        inline void DrawText(chars_t text, u32 x, u32 y)
        {
            auto& meshView = vertexGroups.AppendReturn();
            meshView.v1 = vertices.Count();
            meshView.i1 = indices.Count();

            for(auto i=0; text[i] != '\0'; ++i)
            {
                const auto fw = 14;
                const auto fh = 18; 
                const auto s  = 1; //scale

                const auto& coords = res::MAPPING.GetValue(text[i]);
                const auto quad = res::CreateRect<res::Indexed::Yes>(
                    Rect<int>{i * fw * s, 0, fw * s, fh * s}, 
                    Rect<int>{coords[Vx] * fw, coords[Vy] * fh, fw, fh}
                );

                vertices.AppendArray(quad.verts);
                const u32 c = vertices.Count();
                uint32_t idxs[] {c + 0, c + 1, c + 2, c + 2, c + 3, c + 0};
                indices.AppendArray(idxs);
            }

            meshView.v2 = vertices.Count() - 1;
            meshView.i2 = indices.Count()  - 1;
        }

        //? whole renderer is on heap
        box::Array<Vertex, 1000>        vertices; 
        box::Array<uint32_t, 1500>      indices;
        box::Array<VertexGroup, 100>    vertexGroups; //outlines vertex and index array
    };

}//ns