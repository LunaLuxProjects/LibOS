#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "../../Interface/Headers/AbstractGraphicsContex.hpp"
#    include "DirectScreen.hpp"

AbstractWindowPlatform DirectScreen::getPlatform() const noexcept
{
    return AbstractWindowPlatform::DIRECT_SCREEN_WINDOW;
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

losPosition DirectScreen::losRequestMousePosition() const noexcept
{
    return {0, 0};
}

losPosition DirectScreen::losRequestMouseWheelDelta() const noexcept
{
    return {0, 0};
}

losPosition DirectScreen::losIsBeingPressed() const noexcept
{
    return {0, 0};
}

void DirectScreen::losDestroyKeyboard() noexcept
{
}

void DirectScreen::losDestroyMouse() noexcept
{
}

void DirectScreen::losDestroyWindow() noexcept
{
}

losResult DirectScreen::losCreateWindowSurface(AbstractGraphicsContext*) noexcept
{
    return LOS_SUCCESS;
}
#endif