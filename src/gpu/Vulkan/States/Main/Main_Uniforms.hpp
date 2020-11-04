//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/Objects/BufferExt.hpp"
#include "gpu/Vulkan/Objects/Descriptors.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "res/Resources_Terrain.hpp"
#include "res/Resources.hpp"

namespace rpg::gpu::vuk {

struct Main_Uniforms
{
    //TODO: more clean-up

    enum Bindings : uint32_t
    {
        //meta
        BindingMeta,
        //instances
        BindingInstanceDatas,
        BINDING_INSTANCE_DATAS_END = BindingInstanceDatas + (idx_t) res::MeshMaterialEnum::ENUM_END - 1,
        //shadows
        BindingSun,
        BindingShadowMap,
        //terrain
        BindingTerrainTriangleNormals,
        BindingTerrainTriangleColors,
        //end
        ENUM_END,
    };

    UniformInfo infos [Bindings::ENUM_END];
    Descriptors descriptors;

    using RD = RenderData_Main;
    //meta
    UniformBuffer<RD::Meta, 1> uboMeta;
    //instances
    StorageBuffer<RD::InstanceData, RD::MESH_INSTANCES_MAX> sboInstanceDatas [(idx_t) res::MeshMaterialEnum::ENUM_END];
    //shadow
    VkSampler shadowMapSampler;
    //terrain
    using TerrainMesh = decltype(res::Resources_Terrain::TERRAIN_T::QUADRANT_T::mesh);
    StorageBuffer<com::Vec3f, TerrainMesh::TRIANGLE_COUNT * res::Resources_Terrain::QUADRANT_COUNT_TOTAL> sboTerrainTriangleNormals;
    StorageBuffer<com::Vec4f, TerrainMesh::TRIANGLE_COUNT * res::Resources_Terrain::QUADRANT_COUNT_TOTAL> sboTerrainTriangleColors;


    void Create(VkCommandPool cmdPool, Buffer& uboSun, Image& shadowMaps, res::Resources_Textures& resTextures)
    {
        //? meta
        uboMeta.Create();
        infos[BindingMeta] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = BindingMeta,
                .descriptorType     = uboMeta.DESCRIPTOR_TYPE,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
                .pImmutableSamplers = nullptr,
            },
            .bufferInfo = {
                .buffer = uboMeta.activeBuffer->buffer,
                .offset = 0,
                .range  = VK_WHOLE_SIZE,
            }
        };

        //? instances
        FOR_C_ARRAY(sboInstanceDatas, i)
        {
            auto& ubo = sboInstanceDatas[i];
            ubo.Create();
            infos[BindingInstanceDatas + i] = {
                .type = UniformInfo::Buffer,
                .binding {
                    .binding            = BindingInstanceDatas + (uint32_t)i,
                    .descriptorType     = ubo.DESCRIPTOR_TYPE,
                    .descriptorCount    = 1,
                    .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
                    .pImmutableSamplers = nullptr,
                },
                .bufferInfo = {
                    .buffer = ubo.activeBuffer->buffer,
                    .offset = 0,
                    .range  = VK_WHOLE_SIZE,
                }
            };
        }       

        //? sun
        infos[BindingSun] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = BindingSun,
                .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
                .pImmutableSamplers = nullptr,
            },
            .bufferInfo = {
                .buffer = uboSun.buffer,
                .offset = 0,
                .range  = VK_WHOLE_SIZE,
            }
        };

        //? shadow map
        const VkSamplerCreateInfo shadowSamplerInfo 
        {
            .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext                   = nullptr,
            .flags                   = 0,
            .magFilter               = VK_FILTER_LINEAR,
            .minFilter               = VK_FILTER_LINEAR, 
            .mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 
            .addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 
            .addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
            .mipLodBias              = 0, 
            .anisotropyEnable        = VK_FALSE, 
            .maxAnisotropy           = 0, 
            .compareEnable           = VK_TRUE,
            .compareOp               = VK_COMPARE_OP_LESS, 
            .minLod                  = 0,
            .maxLod                  = 0, 
            .borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK, 
            .unnormalizedCoordinates = VK_FALSE
        };
        VkCheck(vkCreateSampler(g_contextPtr->device, &shadowSamplerInfo, nullptr, &shadowMapSampler));

        infos[BindingShadowMap] = {
            .type = UniformInfo::Image,
            .binding {
                .binding            = BindingShadowMap,
                .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
                .pImmutableSamplers = nullptr,
            },
            .imageInfo {
                .sampler        = shadowMapSampler,
                .imageView      = shadowMaps.view,
                .imageLayout    = shadowMaps.currentLayout
            }
        };

        //? terrain faces
        sboTerrainTriangleNormals.Create();
        infos[BindingTerrainTriangleNormals] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = BindingTerrainTriangleNormals,
                .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
                .pImmutableSamplers = nullptr,
            },
            .bufferInfo = {
                .buffer = sboTerrainTriangleNormals.activeBuffer->buffer,
                .offset = 0,
                .range  = VK_WHOLE_SIZE,
            }
        };

        sboTerrainTriangleColors.Create();
        infos[BindingTerrainTriangleColors] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = BindingTerrainTriangleColors,
                .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
                .pImmutableSamplers = nullptr,
            },
            .bufferInfo = {
                .buffer = sboTerrainTriangleColors.activeBuffer->buffer,
                .offset = 0,
                .range  = VK_WHOLE_SIZE,
            }
        };

        //? DESCRIPTORS
        descriptors.Create(infos);
    }

    void Update(RenderData_Main& rdGeneral, res::Resources_Terrain const& resTerrain)
    {
        //?meta
        uboMeta.Reset();
        uboMeta.Append(rdGeneral.meta);

        //?instances
        auto const& materials = rdGeneral.instance_datas;
        for(idx_t matIdx = 0; matIdx < (idx_t) res::MeshMaterialEnum::ENUM_END; ++matIdx)
        {
            auto& sbo = sboInstanceDatas[matIdx];
            sbo.Reset();
            auto const& meshes = materials[matIdx];
            FOR_C_ARRAY(meshes, meshIdx)
            {
                auto const& instances = meshes[meshIdx];
                if (instances.Empty() == false)
                    sbo.Append(instances);   
            }
        }

        //?terrain faces
        //TODO: only dirty ones
        auto const& quadrants = resTerrain.terrain.quadrants;

        sboTerrainTriangleNormals.Reset();
        FOR_C_ARRAY(quadrants, i)
        {
            auto const& mesh = quadrants[i].mesh;
            sboTerrainTriangleNormals.Append(mesh.triangleNormals);
        }

        sboTerrainTriangleColors.Reset();
        FOR_C_ARRAY(quadrants, i)
        {
            auto const& mesh = quadrants[i].mesh;
            sboTerrainTriangleColors.Append(mesh.triangleColors);
        }
    }

    void Destroy()
    {
        uboMeta.Destroy();
        sboTerrainTriangleNormals.Destroy();
        sboTerrainTriangleColors.Destroy();
        FOR_C_ARRAY(sboInstanceDatas, i)
        {
            auto& ubo = sboInstanceDatas[i];
            ubo.Destroy();
        }
        descriptors.Destroy();
        vkDestroySampler(g_contextPtr->device, shadowMapSampler, nullptr);
        FOR_C_ARRAY(infos, i)
            infos[i] = {};
    }
    ~Main_Uniforms()
    {
        Destroy();
    }

};

}//ns



/*
//? FOLIAGE IMAGES
constexpr VkComponentMapping foliageComponentMapping = 
{ 
    .r = VK_COMPONENT_SWIZZLE_B,
    .g = VK_COMPONENT_SWIZZLE_G,
    .b = VK_COMPONENT_SWIZZLE_R,
    .a = VK_COMPONENT_SWIZZLE_A
};

foliageImages.Create(
    VK_FORMAT_R8_SRGB,
    resTextures.foliage.WIDTH,
    resTextures.foliage.HEIGHT,
    VK_SAMPLE_COUNT_1_BIT, 
    VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
    VK_IMAGE_ASPECT_COLOR_BIT,
    resTextures.foliage.count,
    VK_IMAGE_VIEW_TYPE_2D_ARRAY,
    foliageComponentMapping
);     
foliageImages.Store(
    cmdPool, 
    resTextures.foliage.data, 
    resTextures.foliage.ALL_TEXTURES_BYTES,
    resTextures.foliage.SINGLE_TEXTURE_BYTES
);
foliageImages.Bake(cmdPool);

const auto foliageSamplerInfo = SamplerCreateInfo(
    VK_FILTER_LINEAR,
    VK_SAMPLER_MIPMAP_MODE_NEAREST,
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
);
VkCheck(vkCreateSampler(g_contextPtr->device, &foliageSamplerInfo, nullptr, &foliageSampler));

infos[BindingFoliage] = {
    .type = UniformInfo::Image,
    .binding {
        .binding            = BindingFoliage,
        .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount    = 1,
        .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr,
    },
    .imageInfo {
        .sampler        = foliageSampler,
        .imageView      = foliageImages.view,
        .imageLayout    = foliageImages.currentLayout
    }
};

*/