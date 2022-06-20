#pragma once
#include "../InternalRefractile.h"
#include <thread>

struct refShaderProgram_T
{
    VkPipeline pipe;
    VkPipelineLayout pipe_layout;
};

struct refDrawPass_T
{
    VkRenderPass pass;
};

struct refFrameBuffer_T
{
    VkFramebuffer buffer;
};

struct refCommandBuffer_T
{
    std::thread::id creator;
    VkCommandPool command_pool;
    VkCommandBuffer buffer;
    VkFence render_fence;
    uint32 index = 0;
};