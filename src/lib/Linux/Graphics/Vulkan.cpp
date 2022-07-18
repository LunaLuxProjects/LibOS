#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "../../Interface/Headers/StructsDefines.hpp"
#    include "VkComponents/CVkInstance.hpp"
#    include "VkComponents/CVkPhysicalDeviceList.hpp"
#    include "VkComponents/CVkDevice.hpp"
#    include "VkComponents/CVkMemoryManager.hpp"
#    include "Vulkan.hpp"

VulkanContext::VulkanContext(const data_size dev, losWindow window)
{
    refGraphicsDeviceProperties prop;
    getGraphicsDeviceProperties(dev, &prop);
    bool enforce_one_three = false;
    if (prop.dev_type == REF_DEV_TYPE_DISCRETE_GPU)
        enforce_one_three = true;
    instance = new CVkInstance(this, enforce_one_three);
    window->window->losCreateWindowSurface(this);
    physical_device = CVkPhysicalDeviceList(this, instance)[dev];
    device = new CVkDevice(this);
    memory_manager = new CVkMemoryManager(this);
}

losResult VulkanContext::getGraphicsDeviceProperties(const data_size dev_index, refGraphicsDeviceProperties *prop)
{
    CVkInstance *instance = new CVkInstance(this);

    VkPhysicalDeviceProperties info;
    vkGetPhysicalDeviceProperties(CVkPhysicalDeviceList(this, instance)[dev_index], &info);
    delete instance;

    prop->name = info.deviceName;
    switch (info.deviceType)
    {
    case VK_PHYSICAL_DEVICE_TYPE_OTHER:
        prop->dev_type = REF_DEV_TYPE_OTHER;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        prop->dev_type = REF_DEV_TYPE_DISCRETE_GPU;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        prop->dev_type = REF_DEV_TYPE_INTEGRATED_GPU;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
        prop->dev_type = REF_DEV_TYPE_VIRTUAL_GPU;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
        prop->dev_type = REF_DEV_TYPE_CPU;
        break;
    default:
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
    }
    return LOS_SUCCESS;
}

uint32 VulkanContext::getDeviceCount() noexcept
{
    uint32 ret = 0;
    try
    {
        ret = ~CVkPhysicalDeviceList(this, new CVkInstance(this));
    }
    catch (const std::exception &)
    {
        return ret;
    }
    return ret;
}

CVkInstance *VulkanContext::getInst() const noexcept
{
    return instance;
}

VkPhysicalDevice VulkanContext::getPhys() const noexcept
{
    return physical_device;
}

CVkDevice *VulkanContext::getDev() const noexcept
{
    return device;
}

CVkMemoryManager* VulkanContext::getMem() const noexcept
{
    return memory_manager;
}

VkSwapchainKHR VulkanContext::getSChain() const noexcept
{
    return VK_NULL_HANDLE;
}

VulkanContext::~VulkanContext()
{
    if(instance == nullptr) return;
    delete memory_manager;
    delete device;
    vkDestroySurfaceKHR(~(*instance),surface,nullptr);
    delete instance;
}
#endif