#include "../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#include "../InternalRefractile.hpp"
#include <AL/alut.h>
#include <RefractileAPI.h>

losResult refAppendAudioContext(refHandle)
{
    alutInit(NULL,NULL);
    return LOS_SUCCESS;
}

losResult refUnAppendAudioContext(refHandle)
{
    alutExit();
    return LOS_SUCCESS;
}
#endif