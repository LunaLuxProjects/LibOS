#pragma once
#include <Components/Defines.h>
#include <RefractileAPI.h>

//this is just a base class to act as a lite abstraction layer
class AbstractGraphicsContext
{
  public:
    // virtual destructor
    virtual ~AbstractGraphicsContext() noexcept = 0;
};