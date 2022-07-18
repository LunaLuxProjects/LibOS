#pragma once
#include <libos/Defines.h>
#include "Abstracts.hpp"
struct refHandle_T
{
    bool using_graphics_system = false; 
    bool using_audio_system = false;
};

struct losWindow_T
{
    AbstractWindow* window;
};

struct refGraphicsDevice_T
{
    AbstractGraphicsContext* context;
};

struct refAudioDevice_T
{
    AbstractAudioManager* manager;
};

struct refAudioPlayer_T
{
    uint64 player_id;
};

struct refAudioBuffer_T
{
    uint64 player_id;
};