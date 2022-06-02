#pragma once
#include "Cmake.h"
#include "Callbacks.h"
#include "Graphics/vkExternal.h"
#include <AL/al.h>
#include <AL/alc.h>

struct refHandle_T
{
    VmaAllocator vulkan_allocator;
    vkb::Instance instance;
    vkb::PhysicalDevice physical;
    vkb::Device device;
    vkb::Swapchain swap_chain;
    VkSurfaceKHR surface;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkSurfaceFormatKHR surface_format {VK_FORMAT_MAX_ENUM,VK_COLOR_SPACE_MAX_ENUM_KHR};
    ALCcontext* audio_context;
};