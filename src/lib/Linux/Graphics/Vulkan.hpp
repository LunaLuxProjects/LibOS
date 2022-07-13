#pragma once
#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#include "Graphics/vkExternal.hpp"
#include "../../Graphics/GraphicContext.hpp"
class VulkanContext : public GraphicsContext
{
    VmaAllocator vulkan_allocator;
    VkInstance instance;
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR surface_format {VK_FORMAT_MAX_ENUM,VK_COLOR_SPACE_MAX_ENUM_KHR};
    VkPhysicalDevice physical;
    uint32 graphic_family_index;
    uint32 present_family_index;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkDevice device;
    VkSwapchainKHR swap_chain;
    std::vector<VkImage> swap_chain_images{};
    std::vector<VkImageView> swap_chain_image_views{};
    std::vector<refFrameBuffer> framebuffer{};
    VkSemaphore present_semaphore;
    VkSemaphore render_semaphore;
    VkRenderPass pass;
    VmaAllocation depth_image_alloc;
    VkImage depth_image;
    VkImageView depth_image_view;
    ALCcontext* audio_context;
    refSampleCount sample_count;
    VkExtent2D current_screen_size;
    public:
}

const char *getError(VkResult);
bool check(VkResult,refHandle);
#define VK_TEST(func,ret_val) [[unlikely]]if(check((result = func),handle)) { printf("LIB OS: Vulkan Error: %s\n", getError(result)); return ret_val;}
#endif