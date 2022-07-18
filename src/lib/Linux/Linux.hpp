#pragma once
#include "../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#include <libos/Defines.h>
#include <unistd.h>
#include <libgen.h>

inline const char* getCurrentPath()
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