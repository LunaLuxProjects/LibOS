#include "../Cmake.h"
#if  CMAKE_SYSTEM_NUMBER == 1 || CMAKE_SYSTEM_NUMBER == 2
#include <Components/Defines.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

inline const std::string getCurrentPath()
{
    std::string path = new char[260];
    GetModuleFileNameA(NULL, (LPSTR)path.c_str(), 260);
    return path;
}

inline void lib_panic(const char* msg)
{
    printf("OS LIB PANIC: %s\n", msg);
    exit(-1);
}
#endif