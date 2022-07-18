#include "../../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "../../../Interface/Headers/AbstractGraphicsContex.hpp"
#    include "../Vulkan.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wunused-function"
#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>
#undef VMA_IMPLEMENTATION
#pragma GCC diagnostic pop
#    include "CVkMemoryManager.hpp"

CVkMemoryManager::CVkMemoryManager(AbstractGraphicsContext *handle)
{
    VulkanContext *context = static_cast<VulkanContext *>(handle);
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_3;
    allocatorInfo.physicalDevice = context->getPhys();
    allocatorInfo.device = ~(*context->getDev());
    allocatorInfo.instance = ~(*context->getInst());

    if (handle->check(vmaCreateAllocator(&allocatorInfo, &vkallocator)))
    {
        printf("LIB OS: libos could not create the memory machine from the chosen states.\n");
        throw std::runtime_error("LIB OS: libos could not create the memory machine from the chosen states.\n");
    }
}

CVkMemoryManager::~CVkMemoryManager() noexcept
{
    vmaDestroyAllocator(vkallocator);
}

#endif