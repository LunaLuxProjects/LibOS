#pragma once
#include <Components/Window.h>
#include "../../Callbacks.h"

typedef enum LinuxWindowPlatform
{
    DIRECT_SCREEN_WINDOW,
    WAYLAND_WINDOW,
    XCB_WINDOW
} LinuxWindowPlatform;

class LinuxWindow
{
    public:
    ResizeCallbackFunction resize_callback;
    requestObjectCallback object_callback;
    virtual uint8 getPlatform() const noexcept = 0;
    virtual losResult losCreateKeyboard() noexcept = 0;
    virtual losResult losCreateMouse() noexcept = 0;
    virtual losResult losCreateTouch() noexcept = 0;
    virtual bool hasWindowClosed() const noexcept = 0;
    virtual losResult losUpdateWindow() noexcept = 0;
    virtual bool losIsKeyDown(losKeyboardButton) const noexcept = 0;
    virtual bool losIsMouseDown(losMouseButton) const noexcept = 0;
    virtual losResult losRequestClose() noexcept = 0;
    virtual losPosition losRequestMousePosition() noexcept = 0;
    virtual losPosition losRequestMouseWheelDelta() noexcept = 0;
    virtual losPosition losIsBeingPressed() const noexcept = 0;
    virtual losResult losDestroyKeyboard() noexcept = 0;
    virtual losResult losDestroyMouse()noexcept = 0;
    virtual losResult losDestroyTouch() noexcept = 0;
    virtual losResult losDestroyWindow() noexcept = 0;

    virtual losResult losCreateVulkanSurface(refHandle handle) noexcept = 0;
    virtual ~LinuxWindow() {};
};