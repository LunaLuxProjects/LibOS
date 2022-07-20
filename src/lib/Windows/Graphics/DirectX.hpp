#pragma once
#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 1 || CMAKE_SYSTEM_NUMBER == 2
#    include "../../Interface/Headers/AbstractGraphicsContex.hpp"
#    include <libos/Window.h>
#include <d3d12.h>
#include <stdexcept>
class DirectXContext : public AbstractGraphicsContext
{
  public:
    explicit DirectXContext() = default;
    DirectXContext(const data_size, losWindow);
    virtual losResult getGraphicsDeviceProperties(const data_size, refGraphicsDeviceProperties *);
    virtual uint32 getDeviceCount() noexcept;
    virtual bool check(int64_slow) noexcept override final;
    virtual ~DirectXContext() override final;
};
#    define GET_DEVICE_COUNT(x) x = DirectXContext().getDeviceCount();
#    define GRAPHICS_CONTEXT(x, y) DirectXContext(x, y)
#    define GET_DEVICE_PROPERTIES(dev, prop)                                       \
        if (DirectXContext().getGraphicsDeviceProperties(dev, prop) != LOS_SUCCESS) \
            return LOS_ERROR_HANDLE_LOSSED;
#endif