#pragma once
#include "vkExternal.hpp"
#include "../InternalRefractile.hpp"
class VkVulkanPipelineBuilder
{
    VkPipelineVertexInputStateCreateInfo vertex_input{};
    VkPipelineInputAssemblyStateCreateInfo input_assembly{};
    VkPipelineViewportStateCreateInfo viewport_state{};
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    VkPipelineMultisampleStateCreateInfo multi_sampling{};
    VkPipelineDepthStencilStateCreateInfo depth_stencil{};
    VkPipelineColorBlendStateCreateInfo color_blending{};
    VkPipelineDynamicStateCreateInfo dynamic_state{};
    bool has_depth_stencil = false;
  public:
    explicit VkVulkanPipelineBuilder()
    {
        VkPipelineColorBlendAttachmentState color_blend_attachment{};
        color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        color_blend_attachment.blendEnable = VK_FALSE;

        (void)setVertexInput();
        (void)setInputAssembly();
        (void)setViewportState();
        (void)setRasterizer();
        (void)setMultiSampling();
        (void)setColorBlending(&color_blend_attachment);
        (void)setDynamicState();
    };

    VkVulkanPipelineBuilder& setVertexInput(const uint32 vertexBindingDescriptionCount = 0,const VkVertexInputBindingDescription* pVertexBindingDescriptions = nullptr,
                                            const uint32 vertexAttributeDescriptionCount = 0,const VkVertexInputAttributeDescription* pVertexAttributeDescriptions = nullptr)
    {
        vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input.pNext = nullptr;
        vertex_input.flags = 0;
        vertex_input.pVertexAttributeDescriptions = pVertexAttributeDescriptions;
        vertex_input.pVertexBindingDescriptions = pVertexBindingDescriptions;
        vertex_input.vertexBindingDescriptionCount = vertexBindingDescriptionCount;
        vertex_input.vertexAttributeDescriptionCount = vertexAttributeDescriptionCount;
        return *this;
    }

    VkVulkanPipelineBuilder& setInputAssembly(VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,VkBool32 primitiveRestartEnable = VK_FALSE)
    {
        input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly.pNext = nullptr;
        input_assembly.flags = 0;
        input_assembly.topology = topology;
        input_assembly.primitiveRestartEnable = primitiveRestartEnable;
        return *this;
    }

    VkVulkanPipelineBuilder& setViewportState(const uint32 viewportCount = 1,const VkViewport* pViewports = nullptr,
                                              const uint32 scissorCount = 1, const VkRect2D* pScissors = nullptr)
    {
        viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state.pNext = nullptr;
        viewport_state.flags = 0;
        viewport_state.viewportCount = viewportCount;
        viewport_state.scissorCount = scissorCount;
        viewport_state.pViewports = pViewports;
        viewport_state.pScissors = pScissors;
        return *this;
    }

    VkVulkanPipelineBuilder& setRasterizer(const VkBool32 depthClampEnable = VK_FALSE,const VkBool32 rasterizerDiscardEnable = VK_FALSE,
                                           const VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL,const VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT,
                                           const VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE,VkBool32 depthBiasEnable = VK_FALSE, const float depthBiasConstantFactor = 0,
                                           const float depthBiasClamp = 0,const float depthBiasSlopeFactor = 0,const float lineWidth = 1.0f)
    {
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.pNext = nullptr;
        rasterizer.flags = 0;
        rasterizer.cullMode = cullMode;
        rasterizer.depthBiasClamp = depthBiasClamp;
        rasterizer.depthBiasConstantFactor = depthBiasConstantFactor;
        rasterizer.depthBiasEnable = depthBiasEnable;
        rasterizer.depthBiasSlopeFactor = depthBiasSlopeFactor;
        rasterizer.depthClampEnable = depthClampEnable;
        rasterizer.frontFace = frontFace;
        rasterizer.lineWidth = lineWidth;
        rasterizer.polygonMode = polygonMode;
        rasterizer.rasterizerDiscardEnable = rasterizerDiscardEnable;
        return *this;
    }

    VkVulkanPipelineBuilder& setMultiSampling(const VkSampleCountFlagBits rasterizationSamples = VK_SAMPLE_COUNT_1_BIT, 
                                              const VkBool32 sampleShadingEnable = VK_FALSE,const float minSampleShading = 0,const VkSampleMask* pSampleMask = nullptr,
                                              const VkBool32 alphaToCoverageEnable = VK_FALSE,const VkBool32 alphaToOneEnable = VK_FALSE)
    {
        multi_sampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multi_sampling.pNext = nullptr;
        multi_sampling.flags = 0;
        multi_sampling.rasterizationSamples = rasterizationSamples;
        multi_sampling.sampleShadingEnable = sampleShadingEnable;
        multi_sampling.minSampleShading = minSampleShading;
        multi_sampling.pSampleMask = pSampleMask;
        multi_sampling.alphaToCoverageEnable = alphaToCoverageEnable;
        multi_sampling.alphaToOneEnable = alphaToOneEnable;
        return *this;
    }

    VkVulkanPipelineBuilder& setDepthStencil()
    {
        //TODO: add variables
        has_depth_stencil = true;
        depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depth_stencil.depthTestEnable = VK_TRUE;
        depth_stencil.depthWriteEnable = VK_TRUE;
        depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depth_stencil.depthBoundsTestEnable = VK_FALSE;
        depth_stencil.stencilTestEnable = VK_FALSE;
        return *this;
    }

    VkVulkanPipelineBuilder& setColorBlending(const VkPipelineColorBlendAttachmentState* pAttachments,const float blendConstants_0 = 0.0f,
                                              const float blendConstants_1 = 0.0f,const float blendConstants_2 = 0.0f,const float blendConstants_3 = 0.0f,
                                              const VkBool32 logicOpEnable = VK_FALSE, const VkLogicOp logicOp = VK_LOGIC_OP_COPY,const uint32 attachmentCount = 1)
    {
        color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blending.logicOpEnable = logicOpEnable;
        color_blending.logicOp = logicOp;
        color_blending.attachmentCount = attachmentCount;
        color_blending.pAttachments = pAttachments;
        color_blending.blendConstants[0] = blendConstants_0;
        color_blending.blendConstants[1] = blendConstants_1;
        color_blending.blendConstants[2] = blendConstants_2;
        color_blending.blendConstants[3] = blendConstants_3;
        return *this;
    }

    VkVulkanPipelineBuilder& setDynamicState(const uint32 dynamicStateCount = 0,const VkDynamicState* pDynamicStates = nullptr)
    {
        dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state.pNext = nullptr;
        dynamic_state.flags = 0;
        dynamic_state.dynamicStateCount = dynamicStateCount;
        dynamic_state.pDynamicStates = pDynamicStates;
        return *this;
    }

    VkPipeline build(refHandle handle,const VkPipelineShaderStageCreateInfo* shader_stages,const uint32 stage_count,const VkPipelineLayout layout)
    {
        VkResult result;
        VkPipeline graphics_pipeline = VK_NULL_HANDLE;
        VkGraphicsPipelineCreateInfo pipeline_info{};
        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_info.stageCount = stage_count;
        pipeline_info.pStages = shader_stages;
        pipeline_info.pVertexInputState = &vertex_input;
        pipeline_info.pInputAssemblyState = &input_assembly;
        pipeline_info.pViewportState = &viewport_state;
        pipeline_info.pRasterizationState = &rasterizer;
        pipeline_info.pMultisampleState = &multi_sampling;
        if(has_depth_stencil)
            pipeline_info.pDepthStencilState = &depth_stencil;
        pipeline_info.pColorBlendState = &color_blending;
        pipeline_info.pDynamicState = &dynamic_state;
        pipeline_info.layout = layout;
        pipeline_info.renderPass = handle->pass;
        pipeline_info.subpass = 0;
        pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

        [[unlikely]]if(check((result = vkCreateGraphicsPipelines(handle->device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &graphics_pipeline)),handle)) 
            printf("LIB OS: Vulkan Error: %s\n", getError(result));
        return std::move(graphics_pipeline);
    }

    ~VkVulkanPipelineBuilder() = default;
};