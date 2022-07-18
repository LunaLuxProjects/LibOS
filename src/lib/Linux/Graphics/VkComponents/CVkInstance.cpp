#include "../../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "../../../Interface/Headers/AbstractGraphicsContex.hpp"
#    include "../Vulkan.hpp"
#    include "CVkError.hpp"
#    include "CVkInstance.hpp"
#    include "vkProfile.hpp"

CVkInstance::CVkInstance(AbstractGraphicsContext *handle, const bool enforce_one_three)
{
    VkResult result;
    // Check if the profile is supported at instance level
    VkBool32 profile_supported;
    [[unlikely]] if (handle->check(
                         (result = vpGetInstanceProfileSupport(nullptr, &vk_profile_properties, &profile_supported))))
    {
        printf("Vulkan Error: could not check if device meets LibOS's rendering needs\n");
        throw std::runtime_error("Vulkan Error: could not check if device meets LibOS's rendering needs\n");
    }

    if (!profile_supported)
    {
        printf("Vulkan Error: selected device dose not meet LibOS's rendering needs\n");
        throw std::runtime_error("Vulkan Error: selected device dose not meet LibOS's rendering needs\n");
    }

    uint32_slow version = 0;
    [[unlikely]] if (handle->check((result = vkEnumerateInstanceVersion(&version))))
    {
        printf("LIB OS: Vulkan Error: could not get vulkan supported version\n");
        throw std::runtime_error("LIB OS: Vulkan Error: could not get vulkan supported version\n");
    }
    if (version < VK_API_VERSION_1_1)
        version = VK_API_VERSION_1_1;

    if (version < VK_API_VERSION_1_2)
        version = VK_API_VERSION_1_2;

    if (version < VK_API_VERSION_1_3)
        version = VK_API_VERSION_1_3;

    if (enforce_one_three)
    {
        if (version < VK_API_VERSION_1_3)
        {
            printf("Vulkan Error: enforce api v1.3 or above device requirements are not met\n");
            throw std::runtime_error("LIB OS: Vulkan Error: could not get vulkan supported version\n");
        }
    }
    const VkApplicationInfo app_info = {
        VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, "LibOS_app", VK_MAKE_VERSION(1, 0, 0), "RefractileAPI",
        VK_MAKE_VERSION(1, 0, 0),           version};
    const char *extensions[] = {VK_KHR_SURFACE_EXTENSION_NAME, "VK_KHR_wayland_surface", "VK_KHR_xcb_surface"};
    const VkInstanceCreateInfo inst_info{
        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, nullptr, 0, &app_info, 0, nullptr, 3, extensions};
    const VpInstanceCreateInfo i_info{&inst_info, &vk_profile_properties, 0};

    [[unlikely]] if (handle->check((result = vpCreateInstance(&i_info, nullptr, &vkinstance)))) throw std::
        runtime_error(std::string(std::string("Vulkan Error: ") +=
                                  std::to_string(vkresult_translation_table.find(result)) += "\n"));
}

CVkInstance::~CVkInstance() noexcept
{
    vkDestroyInstance(vkinstance, nullptr);
}

#endif
