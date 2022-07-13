#pragma once
#include "AbstractWindow.hpp"
#include "AbstractAudioManager.hpp"
#include <Components/Window.h>

struct losWindow_T
{
    AbstractWindow* window;
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