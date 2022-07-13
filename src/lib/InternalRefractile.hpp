#pragma once
#include "Cmake.h"
#include <RefractileAPI.h>
#include "Graphics/GraphicsContex.hpp"

struct refHandle_T
{
    losWindow *window;
    GraphicsContext * graphics_context;
    bool using_audio_system = false;
    bool closing = false;
};
