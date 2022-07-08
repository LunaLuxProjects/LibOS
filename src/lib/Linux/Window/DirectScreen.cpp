#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "../../Graphics/vkExternal.hpp"
#    include "DirectScreen.hpp"

LinuxWindowPlatform DirectScreen::getPlatform() const noexcept
{
    return LinuxWindowPlatform::DIRECT_SCREEN_WINDOW;
};

DirectScreen::DirectScreen(const char *, losSize)
{
    throw std::runtime_error("DirectScreen is not implemented");
}

losResult DirectScreen::losCreateKeyboard() noexcept
{
    return LOS_SUCCESS;
}

losResult DirectScreen::losCreateMouse() noexcept
{
    return LOS_SUCCESS;
}

losResult DirectScreen::losCreateTouch() noexcept
{
    return LOS_SUCCESS;
}

bool DirectScreen::hasWindowClosed() const noexcept
{
    return false;
}
losResult DirectScreen::losUpdateWindow() noexcept
{
    return LOS_SUCCESS;
}

bool DirectScreen::losIsKeyDown(losKeyboardButton) const noexcept
{
    return false;
}

bool DirectScreen::losIsMouseDown(losMouseButton) const noexcept
{
    return false;
}

losResult DirectScreen::losRequestClose() noexcept
{
    return LOS_SUCCESS;
};

losPosition DirectScreen::losRequestMousePosition() noexcept
{
    return {0, 0};
}

losPosition DirectScreen::losRequestMouseWheelDelta() noexcept
{
    return {0, 0};
}

losPosition DirectScreen::losIsBeingPressed() const noexcept
{
    return {0, 0};
}

losResult DirectScreen::losDestroyKeyboard() noexcept
{
    return LOS_SUCCESS;
}

losResult DirectScreen::losDestroyMouse() noexcept
{
    return LOS_SUCCESS;
}

losResult DirectScreen::losDestroyTouch() noexcept
{
    return LOS_SUCCESS;
}

losResult DirectScreen::losDestroyWindow() noexcept
{
    return LOS_SUCCESS;
}

losResult DirectScreen::losCreateVulkanSurface(refHandle) noexcept
{
    return LOS_SUCCESS;
}
#endif