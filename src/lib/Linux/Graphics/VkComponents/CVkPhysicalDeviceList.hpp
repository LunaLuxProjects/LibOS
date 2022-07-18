#pragma once
#include <vulkan/vulkan_core.h>
#include "../../../Interface/Headers/AbstractGraphicsContex.hpp"
#include "CVkInstance.hpp"

//FIXME: create and lstd version for std::runtime_error
#include <stdexcept>

class CVkPhysicalDeviceList
{
    VkPhysicalDevice* devices;
    uint32_slow dev;
    public:
    explicit CVkPhysicalDeviceList(AbstractGraphicsContext* handle,CVkInstance* instance)
    {
        [[unlikely]] if (handle->check(vkEnumeratePhysicalDevices(~(*instance), &dev, nullptr)))
        {
            printf("LIB OS: could not get the count of devices on the machine.\n");
            throw std::runtime_error("LIB OS: could not get the count of devices on the machine.\n");
        }

        devices = new VkPhysicalDevice[dev];
        [[unlikely]] if (handle->check(vkEnumeratePhysicalDevices(~(*instance), &dev, devices)))
        {
            printf("LIB OS: could not get the devices on the machine.");
            throw std::runtime_error("LIB OS: could not get the devices on the machine.");
        }
    }

    VkPhysicalDevice operator[](const data_size index)
    {
        if(index > dev)
        {
            printf("LIB OS: Hay! why are you trying to access a device that dose not exsice?\n");
            throw std::runtime_error("LIB OS: Hay! why are you trying to access a device that dose not exsice?\n");
        }
        return devices[index];
    }

    uint32 operator ~() const noexcept
    {
        return dev;
    }

    ~CVkPhysicalDeviceList() noexcept
    {
        devices = nullptr;
    }
};
