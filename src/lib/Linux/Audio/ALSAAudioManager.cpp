#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "ALSAAudioManager.hpp"

#    define SUCCESS(x, y)                                              \
        if ((res = x) != S_OK)                                         \
        {                                                              \
            _com_error err(res);                                       \
            LPCTSTR errMsg = err.ErrorMessage();                       \
            printf("LibOS: Audio WASAPI system error - %s\n", errMsg); \
            return y;                                                  \
        }

ALSAAudioManager::ALSAAudioManager(const data_size)
{
  
}

ALSAAudioManager::~ALSAAudioManager()
{
    
}

losResult ALSAAudioManager::getAudioDeviceProperties(const data_size, refAudioDeviceProperties *) noexcept
{
    return LOS_SUCCESS;
}

uint32 ALSAAudioManager::getDeviceCount() noexcept
{
    uint32 ret = 0;
    return ret;
}
#endif