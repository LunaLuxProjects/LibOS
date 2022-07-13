#pragma once
#include <Components/Defines.h>
#include <RefractileAPI.h>

//this is just a base class to act as a lite abstraction layer
class GraphicsContext
{
  public:
    // called when the graphics context is to created
    virtual losResult createContext(refCreateGraphicContextInfo &info) noexcept = 0;
    // called to destroy the graphics context
    virtual void destroyContext() noexcept = 0;
    // virtual destructor
    virtual ~GraphicsContext() noexcept = 0;
};