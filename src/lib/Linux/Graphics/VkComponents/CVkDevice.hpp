#pragma once
#include "../../../Interface/Headers/Abstracts.hpp"
#include "CVkInstance.hpp"
#include <vulkan/vulkan_core.h>

class CVkDevice
{
    VkDevice vkdevice;
    uint32_slow graphic_family_index;
    uint32_slow present_family_index;

  public:
    class CVkSemaphore
    {
      AbstractGraphicsContext ** context_ptr;
      VkSemaphore vksemaphore;
      public:
        explicit CVkSemaphore(AbstractGraphicsContext *);
        VkSemaphore operator~() const noexcept
        {
            return vksemaphore;
        }
        ~CVkSemaphore() noexcept;
    };
    explicit CVkDevice(AbstractGraphicsContext *);
    VkDevice operator~() const noexcept
    {
        return vkdevice;
    }
    ~CVkDevice() noexcept;
};