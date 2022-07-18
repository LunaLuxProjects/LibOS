#include "../../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "../../../Interface/Headers/AbstractGraphicsContex.hpp"
#    include "../Vulkan.hpp"
#    include "CVkDevice.hpp"
#    include "vkProfile.hpp"

CVkDevice::CVkSemaphore::CVkSemaphore(AbstractGraphicsContext *handle)
{
    context_ptr = &handle;
    VulkanContext *context = static_cast<VulkanContext *>(handle);
    VkSemaphoreCreateInfo semaphore_create_info{};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.pNext = nullptr;
    semaphore_create_info.flags = 0;
    if (handle->check(vkCreateSemaphore(~(*context->getDev()), &semaphore_create_info, nullptr, &vksemaphore)))
    {
        printf("LIB OS: libos could not create a semaphore from the chosen states.\n");
        throw std::runtime_error("LIB OS: libos could not create a semaphore from the chosen states.\n");
    }
}

CVkDevice::CVkSemaphore::~CVkSemaphore() noexcept
{
    VulkanContext *context = static_cast<VulkanContext *>(*context_ptr);
    vkDestroySemaphore(~(*context->getDev()), vksemaphore, nullptr);
}

CVkDevice::CVkDevice(AbstractGraphicsContext *handle)
{
    VulkanContext *context = static_cast<VulkanContext *>(handle);
    std::vector<VkQueueFamilyProperties> queueFamilies;
    uint32_slow queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(context->getPhys(), &queueFamilyCount, nullptr);
    queueFamilies.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(context->getPhys(), &queueFamilyCount, queueFamilies.data());
    uint32 i = 0;
    for (const auto &queueFamily : queueFamilies)
    {
        (void)queueFamily;
        VkBool32 presentSupport = false;
        if (handle->check(
                vkGetPhysicalDeviceSurfaceSupportKHR(context->getPhys(), i, context->getSurf(), &presentSupport)))
        {
            printf("LIB OS: libos could not create a device from the chosen states. due to we could no get if a queue "
                   "supports the window.\n");
            throw std::runtime_error("LIB OS: libos could not create a device from the chosen states. due to we could "
                                     "no get if a queue supports the window.\n");
        }
        if (presentSupport)
        {
            present_family_index = i;
            break;
        }
        i++;
    }
    i = 0;
    for (const auto &queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphic_family_index = i;
            break;
        }
        i++;
    }
    float queuePriority = 1.0f;
    const VkDeviceQueueCreateInfo queue_create_info = {
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr, 0, graphic_family_index, 1, &queuePriority};
    const VkDeviceCreateInfo device_create_info = {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, nullptr, 0, 1, &queue_create_info, 0, nullptr, 0, nullptr, nullptr};
    const VpDeviceCreateInfo profile_device_info = {&device_create_info, &vk_profile_properties, 0};
    if (handle->check(vpCreateDevice(context->getPhys(), &profile_device_info, nullptr, &vkdevice)))
    {
        printf("LIB OS: libos could not create a device from the chosen states.\n");
        throw std::runtime_error("LIB OS: libos could not create a device from the chosen states.\n");
    }
}

CVkDevice::~CVkDevice() noexcept
{
    vkDestroyDevice(vkdevice, nullptr);
}

#endif