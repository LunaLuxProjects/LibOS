#pragma once
#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "../../Interface/Headers/AbstractWindow.hpp"
#    include "../../Interface/Headers/Abstracts.hpp"
#    include <libos/Defines.h>
#    include <atomic>
#    include <xcb/xcb.h>

class XcbWindow : public AbstractWindow
{
    xcb_connection_t *con;
    xcb_window_t win;
    xcb_screen_t *screen;
    xcb_intern_atom_reply_t *atom_wm_delete_window;
    std::atomic_bool shouldClose{false}, isInWindow{false}, isRest{false}, keys[256]{}, buttons[3]{};
    std::atomic<uint16> x = 0, y = 0;
  public:
    explicit XcbWindow(const char *title, losSize win_size);
    virtual AbstractWindowPlatform getPlatform() const noexcept final override;
    virtual bool hasWindowClosed() const noexcept final override;
    virtual losResult losUpdateWindow() noexcept final override;
    virtual bool losIsKeyDown(losKeyboardButton) const noexcept final override;
    virtual bool losIsMouseDown(losMouseButton) const noexcept final override;
    virtual losResult losRequestClose() noexcept final override;
    virtual losPosition losRequestMousePosition() const noexcept final override;
    virtual losPosition losRequestMouseWheelDelta() const noexcept final override;
    virtual losPosition losIsBeingPressed() const noexcept final override;
    virtual void losDestroyWindow() noexcept final override;

    virtual losResult losCreateWindowSurface(AbstractGraphicsContext*) noexcept final override;
};
#endif