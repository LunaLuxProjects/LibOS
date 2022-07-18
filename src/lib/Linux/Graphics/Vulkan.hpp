#pragma once
#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "../../Interface/Headers/AbstractGraphicsContex.hpp"
#    include "../../Interface/Headers/AbstractWindow.hpp"
#    include "VkComponents/CVkForwardDeclare.hpp"
#    include <libos/Window.h>
#    include <vulkan/vulkan_core.h>
class AbstractGraphicsContext;
class VulkanContext : public AbstractGraphicsContext
{
    CVkInstance *instance;
    VkPhysicalDevice physical_device;
    CVkDevice *device;
    CVkMemoryManager *memory_manager;
    VkSurfaceKHR surface;
  public:
    explicit VulkanContext() = default;
    VulkanContext(const data_size, losWindow);
    virtual losResult getGraphicsDeviceProperties(const data_size, refGraphicsDeviceProperties *);
    virtual uint32 getDeviceCount() noexcept;
    virtual bool check(int64_slow result) noexcept override final
    {
        if (result == VK_SUCCESS)
            return false;
        if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            if (getWindow()->hasWindowClosed())
            {
                // destroySwapchain(handle);
                //(void)createSwapChain(handle, false);
            }
            return false;
        }
        return true;
    }

    CVkInstance *getInst() const noexcept;
    VkPhysicalDevice getPhys() const noexcept;
    CVkDevice *getDev() const noexcept;
    CVkMemoryManager* getMem() const noexcept;
    VkSurfaceKHR getSurf() const noexcept
    {
        return surface;
    };
    VkSurfaceKHR *setSurf() noexcept
    {
        return &surface;
    };
    VkSwapchainKHR getSChain() const noexcept;

    virtual ~VulkanContext() override final;
};
#    define GET_DEVICE_COUNT(x) x = VulkanContext().getDeviceCount();
#    define GRAPHICS_CONTEXT(x, y) VulkanContext(x, y)
#    define GET_DEVICE_PROPERTIES(dev, prop)                                       \
        if (VulkanContext().getGraphicsDeviceProperties(dev, prop) != LOS_SUCCESS) \
            return LOS_ERROR_HANDLE_LOSSED;
#endif