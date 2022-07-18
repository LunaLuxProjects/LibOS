#pragma once
#include <libos/Defines.h>
#include <vulkan/vulkan_core.h>
#include <lstd/LookUpTable.h>

const lstd::ReadOnlyLookupTable<int32_slow,uint8> vkresult_translation_table {
    {VK_SUCCESS, LOS_SUCCESS},
    {VK_NOT_READY , LOS_ERROR_GRAPHICS_INTERNALLY_HANDLED_ERROR_HAPPEND},
    {VK_TIMEOUT , LOS_ERROR_GRAPHICS_INTERNALLY_HANDLED_ERROR_HAPPEND},
    {VK_EVENT_SET , LOS_ERROR_GRAPHICS_INTERNALLY_HANDLED_ERROR_HAPPEND},
    {VK_EVENT_RESET , LOS_ERROR_GRAPHICS_INTERNALLY_HANDLED_ERROR_HAPPEND},
    {VK_INCOMPLETE , LOS_ERROR_GRAPHICS_INTERNALLY_HANDLED_ERROR_HAPPEND},
    {VK_ERROR_OUT_OF_HOST_MEMORY, LOS_ERROR_GRAPHICS_MEMORY_FULL},
    {VK_ERROR_OUT_OF_DEVICE_MEMORY, LOS_ERROR_GRAPHICS_MEMORY_FULL},
    {VK_ERROR_INITIALIZATION_FAILED, LOS_ERROR_COULD_NOT_INIT},
    {VK_ERROR_DEVICE_LOST, LOS_ERROR_GRAPHICS_LOST},
    {VK_ERROR_MEMORY_MAP_FAILED, LOS_ERROR_GRAPHICS_MEMORY_CORUPTED},
    {VK_ERROR_LAYER_NOT_PRESENT, LOS_ERROR_COULD_NOT_INIT},
    {VK_ERROR_EXTENSION_NOT_PRESENT, LOS_ERROR_COULD_NOT_INIT},
    {VK_ERROR_FEATURE_NOT_PRESENT, LOS_ERROR_COULD_NOT_INIT},
    {VK_ERROR_INCOMPATIBLE_DRIVER, LOS_ERROR_COULD_NOT_INIT},
    {VK_ERROR_TOO_MANY_OBJECTS, LOS_ERROR_GRAPHICS_MEMORY_CORUPTED},
    {VK_ERROR_FORMAT_NOT_SUPPORTED, LOS_ERROR_COULD_NOT_GET_CORRECT_DATA},
    {VK_ERROR_FRAGMENTED_POOL, LOS_ERROR_GRAPHICS_MEMORY_CORUPTED},
    {VK_ERROR_UNKNOWN, LOS_ERROR_UNKNOWN},
    {VK_ERROR_OUT_OF_POOL_MEMORY, LOS_ERROR_GRAPHICS_MEMORY_FULL},
    {VK_ERROR_INVALID_EXTERNAL_HANDLE, LOS_ERROR_HANDLE_LOSSED},
    {VK_ERROR_FRAGMENTATION, LOS_ERROR_GRAPHICS_MEMORY_CORUPTED},
    {VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,LOS_ERROR_HANDLE_LOSSED},
    {VK_PIPELINE_COMPILE_REQUIRED,LOS_ERROR_GRAPHICS_SHADER_ERROR},
    {VK_ERROR_SURFACE_LOST_KHR, LOS_ERROR_GRAPHICS_LOST},
    {VK_ERROR_NATIVE_WINDOW_IN_USE_KHR, LOS_ERROR_HANDLE_IN_USE},
    {VK_SUBOPTIMAL_KHR, LOS_ERROR_GRAPHICS_INTERNALLY_HANDLED_ERROR_HAPPEND},
    {VK_ERROR_OUT_OF_DATE_KHR, LOS_ERROR_GRAPHICS_INTERNALLY_HANDLED_ERROR_HAPPEND},
    {VK_ERROR_INCOMPATIBLE_DISPLAY_KHR, LOS_ERROR_COULD_NOT_INIT},
    {VK_ERROR_VALIDATION_FAILED_EXT,LOS_ERROR_GRAPHICS_BACKEND_UNDEFINED_BEHAVIOR},
    {VK_ERROR_INVALID_SHADER_NV, LOS_ERROR_GRAPHICS_MEMORY_CORUPTED},
    {VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT, LOS_ERROR_GRAPHICS_MEMORY_CORUPTED},
    {VK_ERROR_NOT_PERMITTED_KHR,LOS_ERROR_GRAPHICS_BACKEND_UNDEFINED_BEHAVIOR},
    {VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT, LOS_ERROR_GRAPHICS_LOST},
    {VK_THREAD_IDLE_KHR, LOS_ERROR_GRAPHICS_INTERNALLY_HANDLED_ERROR_HAPPEND},
    {VK_THREAD_DONE_KHR, LOS_ERROR_GRAPHICS_INTERNALLY_HANDLED_ERROR_HAPPEND},
    {VK_OPERATION_DEFERRED_KHR,LOS_ERROR_GRAPHICS_BACKEND_UNDEFINED_BEHAVIOR},
    {VK_OPERATION_NOT_DEFERRED_KHR,LOS_ERROR_GRAPHICS_BACKEND_UNDEFINED_BEHAVIOR},
    {VK_ERROR_COMPRESSION_EXHAUSTED_EXT, LOS_ERROR_GRAPHICS_MEMORY_CORUPTED},
    {VK_ERROR_OUT_OF_POOL_MEMORY_KHR, LOS_ERROR_GRAPHICS_MEMORY_FULL},
    {VK_RESULT_MAX_ENUM, LOS_ERROR_UNKNOWN}
};