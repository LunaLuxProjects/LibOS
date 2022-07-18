#pragma once
#include "../../Interface/Headers/AbstractAudioManager.hpp"

class ALSAAudioManager : public AbstractAudioManager
{
  public:
    explicit ALSAAudioManager() = default;
    ALSAAudioManager(const data_size);
    virtual losResult getAudioDeviceProperties(const data_size, refAudioDeviceProperties *) noexcept override final;
    virtual uint32 getDeviceCount() noexcept override final;
    virtual ~ALSAAudioManager() override final;
};

#define GET_DEVICE_COUNT(x) x = ALSAAudioManager().getDeviceCount();
#define AUDIO_MANAGER(x) ALSAAudioManager(x)
#define GET_DEVICE_PROPERTIES(dev, prop)                                       \
    if (ALSAAudioManager().getAudioDeviceProperties(dev, prop) != LOS_SUCCESS) \
        return LOS_ERROR_HANDLE_LOSSED;
        