#pragma once
#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "LinuxWindow.h"
#    include <Components/Defines.h>
#    include <atomic>
#    include <xcb/xcb.h>

class XcbWindow : public LinuxWindow
{
    xcb_connection_t *con;
    xcb_window_t win;
    xcb_screen_t *screen;
    xcb_intern_atom_reply_t *atom_wm_delete_window;
    std::atomic_bool shouldClose{false}, isInWindow{false}, isRest{false}, keys[256]{}, buttons[3]{};
    std::atomic<int64> x = 0, y = 0;
  public:
    explicit XcbWindow(const char *title, losSize win_size);
    virtual uint8 getPlatform() const noexcept final override;
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