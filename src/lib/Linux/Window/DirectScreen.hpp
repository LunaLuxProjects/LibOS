#pragma once
#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "LinuxWindow.hpp"
#    include <Components/Defines.h>

class DirectScreen : public LinuxWindow
{
  public:
    explicit DirectScreen(const char *title, losSize win_size);
    virtual LinuxWindowPlatform getPlatform() const noexcept final override;
    virtual losResult losCreateKeyboard() noexcept final override;
    virtual losResult losCreateMouse() noexcept final override;
    virtual losResult losCreateTouch() noexcept final override;
    virtual bool hasWindowClosed() const noexcept final override;
    virtual losResult losUpdateWindow() noexcept final override;
    virtual bool losIsKeyDown(losKeyboardButton) const noexcept final override;
    virtual bool losIsMouseDown(losMouseButton) const noexcept final override;
    virtual losResult losRequestClose() noexcept final override;
    virtual losPosition losRequestMousePosition() noexcept final override;
    virtual losPosition losRequestMouseWheelDelta() noexcept final override;
    virtual losPosition losIsBeingPressed() const noexcept final override;
    virtual losResult losDestroyKeyboard() noexcept final override;
    virtual losResult losDestroyMouse() noexcept final override;
    virtual losResult losDestroyTouch() noexcept final override;
    virtual losResult losDestroyWindow() noexcept final override;

    losResult losCreateVulkanSurface(refHandle handle) noexcept final override;
};
#endif