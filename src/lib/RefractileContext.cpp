#include "InternalRefractile.hpp"
#include <RefractileAPI.h>

losResult refCreateRefractileContext(refHandle *handle)
{
    (*handle) = new refHandle_T();
    return LOS_SUCCESS;
}

void refDestroyRefractileContext(refHandle handle) noexcept
{
    delete handle;
}