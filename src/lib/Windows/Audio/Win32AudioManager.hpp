#pragma once
#include "../../Interface/Headers/AbstractAudioManager.hpp"
#include <audioclient.h>

class Win32AudioManager : public AbstractAudioManager
{
    IAudioClient3 *audio_client;
    WAVEFORMATEX *format = NULL;
  public:
    explicit Win32AudioManager()  = default;
    virtual losResult getAudioDeviceProperties(const size, refAudioDeviceProperties*) noexcept override final;
    virtual uint32 getDeviceCount() noexcept override final;
    Win32AudioManager(const size);
    virtual ~Win32AudioManager() override final;
};

#define GET_DEVICE_COUNT(x) x = Win32AudioManager().getDeviceCount();
#define AUDIO_MANAGER(x) Win32AudioManager(x)
#define GET_DEVICE_PROPERTIES(dev,prop)                                       \
    if (Win32AudioManager().getAudioDeviceProperties(dev,prop) != LOS_SUCCESS) \
        return LOS_ERROR_HANDLE_LOSSED;