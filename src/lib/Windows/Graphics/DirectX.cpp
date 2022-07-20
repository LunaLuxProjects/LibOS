#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 1 || CMAKE_SYSTEM_NUMBER == 2
#include "DirectX.hpp"
DirectXContext::DirectXContext(const data_size, losWindow)
{
}

losResult DirectXContext::getGraphicsDeviceProperties(const data_size, refGraphicsDeviceProperties *)
{
    return losResult();
}

uint32 DirectXContext::getDeviceCount() noexcept
{
    return uint32();
}

bool DirectXContext::check(int64_slow) noexcept
{
    return false;
}

DirectXContext::~DirectXContext()
{
}
#endif

