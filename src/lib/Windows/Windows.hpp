#include "../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 1
#include <Components/Defines.h>
#include <cstdio>
#include <cstdlib>
#include "WindowsLink.hpp"

inline std::string getCurrentPath()
{
    std::string path;
    for (DWORD str_size = 0; str_size < MAX_PATH; str_size++)
    {
        path += " ";
    }
    GetModuleFileName(NULL, (LPSTR)path.c_str(), MAX_PATH);
    return path;
}

inline void lib_panic(const char* msg)
{
    printf("OS LIB PANIC: %s\n", msg);
    exit(-1);
}
#endif