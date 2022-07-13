#include "../../Cmake.h"
#include <Components/Defines.h>

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