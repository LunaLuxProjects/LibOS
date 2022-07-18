#include <libos/RefractileAPI.h>
#include "../../Cmake.h"
#include "../../Interface/Headers/StructsDefines.hpp"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "../../Linux/Audio/ALSAAudioManager.hpp"
#endif
#if CMAKE_SYSTEM_NUMBER == 1
#    include "../../Windows/Audio/WinRTAudioManager.hpp"
#endif
#if CMAKE_SYSTEM_NUMBER == 2
#    include "../../Windows/Audio/Win32AudioManager.hpp"
#endif
#include <stdexcept>

losResult refAppendAudioContext(refHandle handle, data_size * dev_count) noexcept
{
    if (handle->using_audio_system)
        return LOS_ERROR_HANDLE_IN_USE;
    handle->using_audio_system = true;
    GET_DEVICE_COUNT((*dev_count));
    return LOS_SUCCESS;
}

void refUnAppendAudioContext(refHandle handle) noexcept
{
    handle->using_audio_system = false;
}

losResult refGetAudioDeviceProperties(const data_size dev, refAudioDeviceProperties* prop) noexcept
{
    GET_DEVICE_PROPERTIES(dev,prop);
    return LOS_SUCCESS;
}

losResult refRetreveSystemAudioDevice(refAudioDevice *dev, const data_size dev_index)
{
    /* FIXME: this check should stop reusing handles already in use
    if (!(*dev))
        return LOS_ERROR_HANDLE_IN_USE;
    */
       (*dev) = new refAudioDevice_T();
        try
        {
            (*dev)->manager = new AUDIO_MANAGER(dev_index);
        }
        catch (const std::exception &)
        {
            return LOS_ERROR_COULD_NOT_INIT;
        }
        return LOS_SUCCESS;
}

void refReleaseSystemAudioDevice(refAudioDevice dev) noexcept
{
    delete dev->manager;
    delete dev;
}