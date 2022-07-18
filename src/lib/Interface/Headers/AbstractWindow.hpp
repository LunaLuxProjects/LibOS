#pragma once
#include <libos/Window.h>
#include <libos/RefractileAPI.h>
#include <memory>
#include "Abstracts.hpp"
//platform types
enum class AbstractWindowPlatform : uint8
{
    DIRECT_SCREEN_WINDOW = 0x00,
    WAYLAND_WINDOW = 0x01,
    XCB_LIB_WINDOW = 0x02,
    WIN_RT_WINDOW = 0x03,
    WIN32_WINDOW = 0x04,
};

// this is just a base class to act as a lite abstraction layer
class AbstractWindow
{
    protected:
        typedef losResult(*resizeCallbackFunction)(refHandle,uint64, uint64);
        resizeCallbackFunction resize_callback = NULL;
        requestObjectCallback object_callback = NULL;
    public:
    virtual void setResizeCallback(resizeCallbackFunction callback) noexcept
    {
        resize_callback = std::move(callback);
    };

    virtual void setObjectCallback(requestObjectCallback callback) noexcept
    {
        object_callback = std::move(callback);
    };
    virtual AbstractWindowPlatform getPlatform() const noexcept = 0;
    virtual losResult losCreateKeyboard()
    {
        return LOS_SUCCESS;
    };
    virtual losResult losCreateMouse()
    {
        return LOS_SUCCESS;
    };
    virtual losResult losCreateTouch()
    {
        return LOS_SUCCESS;
    };
    virtual bool hasWindowClosed() const noexcept = 0;
    virtual losResult losUpdateWindow() noexcept = 0;
    virtual bool losIsKeyDown(losKeyboardButton) const noexcept = 0;
    virtual bool losIsMouseDown(losMouseButton) const noexcept = 0;
    virtual losResult losRequestClose() noexcept = 0;
    virtual losPosition losRequestMousePosition() const noexcept = 0;
    virtual losPosition losRequestMouseWheelDelta() const noexcept = 0;
    virtual losPosition losIsBeingPressed() const noexcept = 0;
    virtual void losDestroyKeyboard() noexcept {};
    virtual void losDestroyMouse() noexcept {};
    virtual void losDestroyTouch() noexcept {};
    virtual void losDestroyWindow() noexcept = 0;

    virtual losResult losCreateWindowSurface(AbstractGraphicsContext*) noexcept = 0;
    virtual ~AbstractWindow() {};
};
