#pragma once
#include <RefractileAPI.h>

class AbstractAudioManager
{
  public:
    virtual losResult getAudioDeviceProperties(const size, refAudioDeviceProperties*) = 0;
    virtual uint32 getDeviceCount() noexcept = 0;
    virtual ~AbstractAudioManager() = default;
};