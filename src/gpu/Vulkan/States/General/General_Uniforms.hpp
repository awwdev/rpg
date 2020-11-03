//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/Objects/BufferExt.hpp"
#include "gpu/Vulkan/Objects/Descriptors.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "res/Resources_Terrain.hpp"
#include "res/Resources.hpp"

namespace rpg::gpu::vuk {

struct General_Uniforms
{
    enum Bindings : uint32_t {
        BindingMeta                     = 0,
        BindingModelInstances           = 1,
        BindingSun                      = 2,
        BindingShadowMap                = 3,
        BindingFoliage                  = 4,
        BindingFX                       = 5, 
        BindingTerrainTriangleNormals   = 6,
        BindingTerrainTriangleColors    = 7,
        ENUM_END
    };

    UniformInfo infos [Bindings::ENUM_END];
    Descriptors descriptors;

    using RD = RenderData_General;

    UniformBuffer<RD::Meta, 1> uboMeta;
    StorageBuffer<float, RD::MESH_INSTANCES_MAX> sboInstances; //!TODO: need multiple UBOs
    VkSampler shadowMapSampler;

    using TerrainMesh = decltype(res::Resources_Terrain::TERRAIN_T::QUADRANT_T::mesh);
    StorageBuffer<com::Vec3f, TerrainMesh::TRIANGLE_COUNT * res::Resources_Terrain::QUADRANT_COUNT_TOTAL> sboTerrainTriangleNormals;
    StorageBuffer<com::Vec4f, TerrainMesh::TRIANGLE_COUNT * res::Resources_Terrain::QUADRANT_COUNT_TOTAL> sboTerrainTriangleColors;

    Image     foliageImages;
    VkSampler foliageSampler;

    Image     fxImages;;
    VkSampler fxSampler;

    void Create(VkCommandPool cmdPool, Buffer& uboSun, Image& shadowMaps, res::Resources_Textures& resTextures)
    {
        //? UBO META
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

        //? UBO MODELS
        sboInstances.Create();
        infos[BindingModelInstances] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = BindingModelInstances,
                .descriptorType     = sboInstances.DESCRIPTOR_TYPE,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
                .pImmutableSamplers = nullptr,
            },
            .bufferInfo = {
                .buffer = sboInstances.activeBuffer->buffer,
                .offset = 0,
                .range  = VK_WHOLE_SIZE,
            }
        };

        //? SUN
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

        //? SHADOW MAP
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

        //? FX TEXTURES
        fxImages.Create(
            VK_FORMAT_R8G8B8A8_SRGB,
            resTextures.fx.WIDTH,
            resTextures.fx.HEIGHT,
            VK_SAMPLE_COUNT_1_BIT, 
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_IMAGE_ASPECT_COLOR_BIT,
            resTextures.fx.count,
            VK_IMAGE_VIEW_TYPE_2D_ARRAY
        );     
        fxImages.Store(
            cmdPool, 
            resTextures.fx.data, 
            resTextures.fx.ALL_TEXTURES_BYTES,
            resTextures.fx.SINGLE_TEXTURE_BYTES
        );
        fxImages.Bake(cmdPool);

        const auto fxSamplerInfo = SamplerCreateInfo(
            VK_FILTER_NEAREST,
            VK_SAMPLER_MIPMAP_MODE_NEAREST,
            VK_SAMPLER_ADDRESS_MODE_REPEAT
        );
        VkCheck(vkCreateSampler(g_contextPtr->device, &fxSamplerInfo, nullptr, &fxSampler));

        infos[BindingFX] = {
            .type = UniformInfo::Image,
            .binding {
                .binding            = BindingFX,
                .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
                .pImmutableSamplers = nullptr,
            },
            .imageInfo {
                .sampler        = fxSampler,
                .imageView      = fxImages.view,
                .imageLayout    = fxImages.currentLayout
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

    void Update(RenderData_General& rdGeneral, res::Resources_Terrain const& resTerrain)
    {
        //?meta
        uboMeta.Reset();
        uboMeta.Append(rdGeneral.meta);

        //?instances
        sboInstances.Reset();
        //!TODO
        //FOR_C_ARRAY(rdGeneral.meshInstances, i){
        //    if (auto const& instanceData = rdGeneral.meshInstances[i]; 
        //        instanceData.Empty() == false && i != (idx_t) res::MeshEnum::None)
        //        sboInstances.Append(instanceData);
        //}

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
        sboInstances.Destroy();
        descriptors.Destroy();
        vkDestroySampler(g_contextPtr->device, shadowMapSampler, nullptr);
        vkDestroySampler(g_contextPtr->device, foliageSampler, nullptr);
        vkDestroySampler(g_contextPtr->device, fxSampler, nullptr);
        foliageImages.Destroy();
        fxImages.Destroy();
        FOR_C_ARRAY(infos, i)
            infos[i] = {};
    }
    ~General_Uniforms()
    {
        Destroy();
    }

};

}//ns