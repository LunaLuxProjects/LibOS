#include "../../Interface/Headers/AbstractGraphicsContex.hpp"
#include "../../Cmake.h"
#include "../../Interface/Headers/StructsDefines.hpp"
#include <libos/RefractileAPI.h>
#if CMAKE_SYSTEM_NUMBER == 0
#    include "../../Linux/Graphics/Vulkan.hpp"
#endif
#if CMAKE_SYSTEM_NUMBER == 1 || CMAKE_SYSTEM_NUMBER == 2
#    include "../../Windows/Graphics/DirectX.hpp"
#endif

losResult refAppendGraphicsContext(refHandle handle, data_size *dev_count) noexcept
{
    if (handle->using_graphics_system)
        return LOS_ERROR_HANDLE_IN_USE;
    handle->using_graphics_system = true;
    GET_DEVICE_COUNT((*dev_count));
    return LOS_SUCCESS;
}

void refUnAppendGraphicsContext(refHandle handle) noexcept
{
    handle->using_graphics_system = false;
}

losResult refGetGraphicsDeviceProperties(const data_size dev, refGraphicsDeviceProperties *prop) noexcept
{
    GET_DEVICE_PROPERTIES(dev, prop);
    return LOS_SUCCESS;
}

losResult refRetreveSystemGraphicsDevice(refGraphicsDevice *dev, losWindow window, const data_size dev_index)
{
    if (!(*dev))
        return LOS_ERROR_HANDLE_IN_USE;

    (*dev) = new refGraphicsDevice_T();
    try
    {
        (*dev)->context = new GRAPHICS_CONTEXT(dev_index, window);
    }
    catch (const std::exception &)
    {
        return LOS_ERROR_COULD_NOT_INIT;
    }
    return LOS_SUCCESS;
}

void refReleaseSystemGraphicsDevice(refGraphicsDevice dev) noexcept
{
    delete dev->context;
    delete dev;
}