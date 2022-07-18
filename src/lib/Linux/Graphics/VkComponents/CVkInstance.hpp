#pragma once
#include "../../../Interface/Headers/Abstracts.hpp"
#include <vulkan/vulkan_core.h>

class CVkInstance
{
    VkInstance vkinstance;

  public:
    explicit CVkInstance(AbstractGraphicsContext *, const bool enforce_one_three = false);
    VkInstance operator~() const noexcept
    {
        return vkinstance;
    }
    ~CVkInstance() noexcept;
};