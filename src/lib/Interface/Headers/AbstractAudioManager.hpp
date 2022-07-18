#pragma once
#include <libos/RefractileAPI.h>

//this is just a base class to act as a lite abstraction layer
class AbstractAudioManager
{
  public:
    //get a devices properties from device
    virtual losResult getAudioDeviceProperties(const data_size, refAudioDeviceProperties*) noexcept = 0;
    //get a count of devices capable of sound input or output processing
    virtual uint32 getDeviceCount() noexcept = 0;
    // virtual destructor
    virtual ~AbstractAudioManager() = default;
};