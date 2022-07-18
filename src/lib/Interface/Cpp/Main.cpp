#include "../../Cmake.h"
#include <libos/Defines.h>
#include <libos/RefractileAPI.h>
#include "../Headers/StructsDefines.hpp"
losResult refCreateRefractileContext(refHandle *handle)
{
    (*handle) = new refHandle_T();
    return LOS_SUCCESS;
}

void refDestroyRefractileContext(refHandle handle) noexcept
{
    delete handle;
}

#if CMAKE_SYSTEM_NUMBER == 0
void libOSInit()
{
}

void libOSCleanUp()
{
}
#endif
#if CMAKE_SYSTEM_NUMBER == 1
#include <winrt/base.h>

void libOSInit()
{
    winrt::init_apartment();
}

void libOSCleanUp()
{
    winrt::uninit_apartment();
}
#endif
#if CMAKE_SYSTEM_NUMBER == 2
#    include <combaseapi.h>
#    include <comdef.h>
#    define SUCCESS(x)                                              \
        if ((res = x) != S_OK)                                         \
        {                                                              \
            _com_error err(res);                                       \
            LPCTSTR errMsg = err.ErrorMessage();                       \
            printf("LibOS: Audio WASAPI system error - %s\n", errMsg); \
            return;                                                  \
        }


void libOSInit()
{
    HRESULT res;
    SUCCESS(CoInitializeEx(nullptr, COINIT_SPEED_OVER_MEMORY | COINIT_APARTMENTTHREADED))
}

void libOSCleanUp()
{
}
#endif