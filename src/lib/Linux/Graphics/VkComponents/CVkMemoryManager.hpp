#pragma once
#include "../../../Interface/Headers/Abstracts.hpp"
#include "CVkDevice.hpp"
#include "CVkInstance.hpp"
#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wunused-function"
#include <vma/vk_mem_alloc.h>
#pragma GCC diagnostic pop
#include <vulkan/vulkan_core.h>

class CVkMemoryManager
{
    VmaAllocator vkallocator;

  public:
    CVkMemoryManager(AbstractGraphicsContext *);
    VmaAllocator operator~() const noexcept
    {
        return vkallocator;
    }
    ~CVkMemoryManager() noexcept;
};