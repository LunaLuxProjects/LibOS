#pragma once
#include <libos/RefractileAPI.h>
#include "Abstracts.hpp"
// this is just a base class to act as a lite abstraction layer
class AbstractGraphicsContext
{
  protected:
    AbstractWindow **window;
    // get the pointer to the pointer to the window
    AbstractWindow *getWindow() const noexcept
    {
        return *window;
    }
    // set the pointer to the pointer to the window
    void setWindow(AbstractWindow **window_in) noexcept
    {
        window = window_in;
    }
  public:
    // get a devices properties from device
    virtual losResult getGraphicsDeviceProperties(const data_size, refGraphicsDeviceProperties *) = 0;
    // get a count of devices capable of drawing to the screen
    virtual uint32 getDeviceCount() noexcept = 0;
    // error checking
    virtual bool check(int64_slow result) noexcept = 0;
    // virtual destructor
    virtual ~AbstractGraphicsContext() noexcept { window = nullptr; };
};