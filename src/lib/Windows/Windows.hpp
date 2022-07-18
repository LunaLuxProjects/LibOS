#include "../Cmake.h"
#if  CMAKE_SYSTEM_NUMBER == 1 || CMAKE_SYSTEM_NUMBER == 2
#include <lstd/String.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

inline const lstd::string getCurrentPath()
{
    lstd::string path = new char[260];
    GetModuleFileNameA(NULL, (LPSTR)path.c_str(), 260);
    return path;
}

inline void lib_panic(const char* msg)
{
    printf("OS LIB PANIC: %s\n", msg);
    exit(-1);
}
#endif