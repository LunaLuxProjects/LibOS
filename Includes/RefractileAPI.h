#pragma once
// this is (DRAFT 4) of the refractile api
#include "Components/Defines.h"
#include "Components/Window.h"
#include <vector>

DEFINE_HANDLE(refHandle)
EXPORT_DLL losResult refCreateRefractileContext(refHandle *);
EXPORT_DLL void refDestroyRefractileContext(refHandle handle) noexcept;
    //---------------------------------------------------------------------------------------------------------
// The Graphics API
//---------------------------------------------------------------------------------------------------------
DEFINE_HANDLE(refImage)
DEFINE_HANDLE(refDataBuffer)
DEFINE_HANDLE(refFrameBuffer)
DEFINE_HANDLE(refCommandBuffer)
DEFINE_HANDLE(refShaderProgram)

typedef struct refCreateShaderProgramInfo
{
    const char* shader_settings;
    const char* vertex_shader;
    const char* fragment_shader;
    //user is giving direct spirv
    bool pre_compiled = false;
} refCreateShaderProgramInfo;

typedef enum refDataBufferType
{
    REF_TYPE_VERTEX,
    REF_TYPE_INDEX,
} refDataBufferType;

typedef struct refCreateDataBufferInfo
{
    refDataBufferType type;
    void * data;
    size data_size;
} refCreateDataBufferInfo;

typedef enum refSampleCount 
{
    REF_SAMPLE_COUNT_1 = 1,
    REF_SAMPLE_COUNT_2 = 2,
    REF_SAMPLE_COUNT_4 = 4,
    REF_SAMPLE_COUNT_8 = 8,
    REF_SAMPLE_COUNT_16 = 10,
    REF_SAMPLE_COUNT_32 = 20,
    REF_SAMPLE_COUNT_64 = 40
} refSampleCount;

typedef struct refCreateGraphicContextInfo
{
    refSampleCount sample_count = REF_SAMPLE_COUNT_1;
    bool has_depth_stencil = false;
    bool find_best_device = true;
} refCreateGraphicContextInfo;

EXPORT_DLL losResult refAppendGraphicsContext(refHandle,losWindow,const refCreateGraphicContextInfo&) noexcept;
EXPORT_DLL void refUnAppendGraphicsContext(refHandle) noexcept;

EXPORT_DLL losResult refCreateFrameBuffer(refHandle,refFrameBuffer *);
EXPORT_DLL void refDestroyFrameBuffer(refHandle, refFrameBuffer) noexcept;

EXPORT_DLL losResult refCreateCommandBuffer(refHandle, refCommandBuffer *);
EXPORT_DLL void refDestroyCommandBuffer(refHandle, refCommandBuffer) noexcept;

EXPORT_DLL losResult refCreateShaderProgram(refHandle, refShaderProgram *, refCreateShaderProgramInfo &);
EXPORT_DLL void refDestroyShaderProgram(refHandle, refShaderProgram) noexcept;

EXPORT_DLL losResult refBeginCommands(refHandle, refCommandBuffer);
EXPORT_DLL losResult refCmdBeginDrawing(refHandle, refCommandBuffer, refFrameBuffer,const float32[4]);
EXPORT_DLL losResult refCmdBindVertexBuffer(refHandle, refCommandBuffer, refDataBuffer);
EXPORT_DLL losResult refCmdBindIndexBuffer(refHandle, refCommandBuffer, refDataBuffer);
EXPORT_DLL losResult refCmdBindShaderProgram(refHandle, refCommandBuffer, refShaderProgram);
EXPORT_DLL losResult refCmdDraw(refHandle, refCommandBuffer,uint32,uint32,uint32,uint32);
EXPORT_DLL losResult refCmdDrawIndexed(refHandle, refCommandBuffer,uint32,uint32,uint32,uint32,uint32);
EXPORT_DLL losResult refCmdEndDrawing(refHandle, refCommandBuffer);
EXPORT_DLL losResult refEndCommands(refHandle, refCommandBuffer);
EXPORT_DLL losResult refExecuteCommands(refHandle, refCommandBuffer, bool);
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

EXPORT_DLL losResult refAppendAudioContext(refHandle, size*) noexcept;
EXPORT_DLL void refUnAppendAudioContext(refHandle) noexcept;

EXPORT_DLL losResult refGetAudioDeviceProperties(const size, refAudioDeviceProperties*) noexcept;

EXPORT_DLL losResult refRetreveSystemAudioDevice(refAudioDevice *,const size);
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