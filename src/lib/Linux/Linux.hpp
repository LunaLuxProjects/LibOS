#pragma once
#include "../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#include <Components/Defines.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <libgen.h>

inline std::string getCurrentPath()
{
    char result[4096];
    (void)readlink("/proc/self/exe", result, 4096);
    return dirname(result);
}

inline void lib_panic(const char* msg)
{
    printf("OS LIB PANIC: %s\n", msg);
    exit(-1);
}
#endif