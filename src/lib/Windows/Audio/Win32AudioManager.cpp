#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 2
#    include "Win32AudioManager.hpp"
#    include <algorithm>
#    include <cctype>
#    include <combaseapi.h>
#    include <comdef.h>
#    include <initguid.h>
#    include <mmdeviceapi.h>
#    include <Functiondiscoverykeys_devpkey.h>
#    include <string>

#    define SUCCESS(x, y)                                              \
        if ((res = x) != S_OK)                                         \
        {                                                              \
            _com_error err(res);                                       \
            LPCTSTR errMsg = err.ErrorMessage();                       \
            printf("LibOS: Audio WASAPI system error - %s\n", errMsg); \
            return y;                                                  \
        }

//TODO: audio output

Win32AudioManager::Win32AudioManager(const data_size dev)
{
    HRESULT res;
    IMMDeviceEnumerator *_enum_;
    SUCCESS(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(LPVOID *)(&_enum_)),;)
    IMMDeviceCollection *devices;
    SUCCESS(_enum_->EnumAudioEndpoints(eAll, DEVICE_STATE_ACTIVE, &devices), ;)
    IMMDevice *device;
    SUCCESS(devices->Item(static_cast<UINT>(dev), &device), ;)
    SUCCESS(device->Activate(__uuidof(IAudioClient3), CLSCTX_ALL, nullptr, (LPVOID *)(&audio_client)), ;)
    SUCCESS(audio_client->GetMixFormat(&format), ;);
    SUCCESS(audio_client->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 10000000, 0, format, NULL), ;);
    device->Release();
    devices->Release();
    _enum_->Release();
}

Win32AudioManager::~Win32AudioManager()
{
    audio_client->Stop();
    audio_client->Release();
}

losResult Win32AudioManager::getAudioDeviceProperties(const data_size dev, refAudioDeviceProperties *prop) noexcept
{
    HRESULT res;
    IMMDeviceEnumerator *_enum_;
    SUCCESS(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
                             (LPVOID *)(&_enum_)),
            LOS_ERROR_COULD_NOT_INIT)
    IMMDeviceCollection *devices;
    SUCCESS(_enum_->EnumAudioEndpoints(eAll, DEVICE_STATE_ACTIVE, &devices), LOS_ERROR_COULD_NOT_INIT)

    IMMDevice *device;
    SUCCESS(devices->Item(static_cast<UINT>(dev), &device), LOS_ERROR_COULD_NOT_INIT)

    IPropertyStore *property_store;
    PROPVARIANT prop_;
    SUCCESS(device->OpenPropertyStore(STGM_READ, &property_store), LOS_ERROR_COULD_NOT_INIT)
    // name
    PropVariantInit(&prop_);
    SUCCESS(property_store->GetValue(PKEY_Device_FriendlyName, &prop_), LOS_ERROR_COULD_NOT_INIT)
    DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, prop_.pwszVal, -1, NULL, 0, NULL, FALSE);
    prop->name = new char[dwNum];
    WideCharToMultiByte(CP_OEMCP, NULL, prop_.pwszVal, -1, prop->name, dwNum, NULL, FALSE);
    PropVariantClear(&prop_);
    // other
    PropVariantInit(&prop_);
    SUCCESS(property_store->GetValue(PKEY_AudioEngine_DeviceFormat, &prop_), LOS_ERROR_COULD_NOT_INIT)
    WAVEFORMATEX *d_format = reinterpret_cast<WAVEFORMATEX *>(prop_.blob.pBlobData);
    switch (d_format->nChannels)
    {
    case 1:
        prop->audio_channels = REF_CHANNELS_1;
        break;
    case 2:
        prop->audio_channels = REF_CHANNELS_2;
        break;
    case 3:
        prop->audio_channels = REF_CHANNELS_3;
        break;
    case 4:
        prop->audio_channels = REF_CHANNELS_4;
        break;
    case 5:
        prop->audio_channels = REF_CHANNELS_5;
        break;
    case 6:
        prop->audio_channels = REF_CHANNELS_6;
        break;
    case 7:
        prop->audio_channels = REF_CHANNELS_7;
        break;
    case 8:
        prop->audio_channels = REF_CHANNELS_8;
        break;
    default:
        prop->audio_channels = REF_CHANNELS_NULL;
    }

    prop->audio_sample_rate = d_format->nSamplesPerSec;
    PropVariantClear(&prop_);
    PropVariantInit(&prop_);
    SUCCESS(property_store->GetValue(PKEY_AudioEndpoint_FormFactor, &prop_), LOS_ERROR_COULD_NOT_INIT)
    switch (prop_.ulVal)
    {
    case LineLevel:
    case Microphone: 
        prop->audio_exe_type = REF_EXE_TYPE_INPUT;
        break;
    case Speakers:
    case Headphones:
    case SPDIF:
    case DigitalAudioDisplayDevice:
        prop->audio_exe_type = REF_EXE_TYPE_OUTPUT;
        break;
    case Headset:
    case Handset:
    case RemoteNetworkDevice:
    case UnknownDigitalPassthrough:
        prop->audio_exe_type = REF_EXE_TYPE_IO;
        break;
    default:
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
    }
    PropVariantClear(&prop_);
    property_store->Release();
    device->Release();
    devices->Release();
    _enum_->Release();
    return LOS_SUCCESS;
}

uint32 Win32AudioManager::getDeviceCount() noexcept
{
    HRESULT res;
    uint32 ret = 0;
    IMMDeviceEnumerator *_enum_;
    SUCCESS(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
                             (LPVOID *)(&_enum_)),
            ret)
    IMMDeviceCollection *devices;
    SUCCESS(_enum_->EnumAudioEndpoints(eAll, DEVICE_STATE_ACTIVE, &devices), ret)
    SUCCESS(devices->GetCount(&ret), ret)
    devices->Release();
    _enum_->Release();
    return ret;
}
#endif