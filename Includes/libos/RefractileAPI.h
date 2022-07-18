#pragma once
// this is (DRAFT 8) of the refractile api
#include "Defines.h"
#include "Window.h"
DEFINE_HANDLE(refHandle)
EXPORT_DLL losResult refCreateRefractileContext(refHandle *);
EXPORT_DLL void refDestroyRefractileContext(refHandle handle) noexcept;
//---------------------------------------------------------------------------------------------------------
// The Graphics API
//---------------------------------------------------------------------------------------------------------
DEFINE_HANDLE(refGraphicsDevice)

typedef enum refDeviceType
{
    REF_DEV_TYPE_OTHER = 0x000,
    REF_DEV_TYPE_INTEGRATED_GPU = 0x001,
    REF_DEV_TYPE_DISCRETE_GPU = 0x002,
    REF_DEV_TYPE_VIRTUAL_GPU = 0x004,
    REF_DEV_TYPE_CPU = 0x008,
} refDeviceType;

typedef struct refGraphicsDeviceProperties
{
    char *name;
    refDeviceType dev_type;
} refGraphicsDeviceProperties;

EXPORT_DLL losResult refAppendGraphicsContext(refHandle, data_size*) noexcept;
EXPORT_DLL void refUnAppendGraphicsContext(refHandle) noexcept;

EXPORT_DLL losResult refGetGraphicsDeviceProperties(const data_size, refGraphicsDeviceProperties*) noexcept;

EXPORT_DLL losResult refRetreveSystemGraphicsDevice(refGraphicsDevice *,losWindow,const data_size);
EXPORT_DLL void refReleaseSystemGraphicsDevice(refGraphicsDevice) noexcept;
//---------------------------------------------------------------------------------------------------------
// The Audio API
//---------------------------------------------------------------------------------------------------------
DEFINE_HANDLE(refAudioDevice)
DEFINE_HANDLE(refAudioBuffer)
DEFINE_HANDLE(refAudioPlayer)

typedef enum refAudioChannels
{
    REF_CHANNELS_NULL = 0x000,
    REF_CHANNELS_1 = 0x001,
    REF_CHANNELS_2 = 0x002,
    REF_CHANNELS_3 = 0x004,
    REF_CHANNELS_4 = 0x008,
    REF_CHANNELS_5 = 0x016,
    REF_CHANNELS_6 = 0x032,
    REF_CHANNELS_7 = 0x064,
    REF_CHANNELS_8 = 0x128,
} refAudioChannels;

typedef enum refAudioExeType
{
    REF_EXE_TYPE_INPUT = 0x000,
    REF_EXE_TYPE_OUTPUT = 0x001,
    REF_EXE_TYPE_IO = 0x002,
} refAudioExeType;

typedef struct refAudioDeviceProperties
{
    char *name;
    refAudioChannels audio_channels;
    uint32 audio_sample_rate;
    refAudioExeType audio_exe_type;
} refAudioDeviceProperties;

EXPORT_DLL losResult refAppendAudioContext(refHandle, data_size*) noexcept;
EXPORT_DLL void refUnAppendAudioContext(refHandle) noexcept;

EXPORT_DLL losResult refGetAudioDeviceProperties(const data_size, refAudioDeviceProperties*) noexcept;

EXPORT_DLL losResult refRetreveSystemAudioDevice(refAudioDevice *,const data_size);
EXPORT_DLL void refReleaseSystemAudioDevice(refAudioDevice) noexcept;

//---------------------------------------------------------------------------------------------------------
// The Audio Output API
//---------------------------------------------------------------------------------------------------------
EXPORT_DLL losResult refCreateAudioBuffer(refAudioBuffer *, const char *);
EXPORT_DLL void refDestroyAudioBuffer(refAudioBuffer) noexcept;

EXPORT_DLL losResult refCreateAudioBuffer(refAudioBuffer *, const char *);
EXPORT_DLL void refDestroyAudioBuffer(refAudioBuffer) noexcept;

EXPORT_DLL losResult refCreateAudioPlayer(refAudioPlayer *, const refAudioBuffer);
EXPORT_DLL void refDestroyAudioPlayer(refAudioPlayer) noexcept;

EXPORT_DLL losResult refPlay(refAudioPlayer) noexcept;
EXPORT_DLL losResult refPlayAt(refAudioPlayer, const float64, const float64, const float64) noexcept;
EXPORT_DLL losResult refPause(refAudioPlayer) noexcept;
EXPORT_DLL losResult refResume(refAudioPlayer) noexcept;
EXPORT_DLL losResult refStop(refAudioPlayer) noexcept;

//---------------------------------------------------------------------------------------------------------