#pragma once
#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "../../Interface/Headers/AbstractWindow.hpp"
#    include "../../Interface/Headers/Abstracts.hpp"
#    include <libos/Defines.h>

class DirectScreen : public AbstractWindow
{
  public:
    explicit DirectScreen(const char *, losSize);
    virtual AbstractWindowPlatform getPlatform() const noexcept final override;
    virtual losResult losCreateKeyboard() noexcept final override;
    virtual losResult losCreateMouse() noexcept final override;
    virtual bool hasWindowClosed() const noexcept final override;
    virtual losResult losUpdateWindow() noexcept final override;
    virtual bool losIsKeyDown(losKeyboardButton) const noexcept final override;
    virtual bool losIsMouseDown(losMouseButton) const noexcept final override;
    virtual losResult losRequestClose() noexcept final override;
    virtual losPosition losRequestMousePosition() const noexcept final override;
    virtual losPosition losRequestMouseWheelDelta() const noexcept final override;
    virtual losPosition losIsBeingPressed() const noexcept final override;
    virtual void losDestroyKeyboard() noexcept final override;
    virtual void losDestroyMouse() noexcept final override;
    virtual void losDestroyWindow() noexcept final override;

    losResult losCreateWindowSurface(AbstractGraphicsContext *) noexcept final override;
};
#endif