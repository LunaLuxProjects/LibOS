#pragma once
#include "../../Interface/Headers/Abstracts.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include "../../Interface/Headers/AbstractWindow.hpp"

class Win32Window : public AbstractWindow
{
    HWND win_hand = NULL;
    bool should_window_close = false;
    bool mouse_inside_window = false;
    bool key_buttons[256] = {false};
    bool mouse_buttons[3] = {false};
    uint64 mouse_position_x = 0;
    uint64 mouse_position_y = 0;
    float64 mouse_wheel_delta_x = 0;
    float64 mouse_wheel_delta_y = 0;
    const uint16 window_key_look_up_table[256] = {
        0x30,         0x31,          0x32,        0x33,        0x34,       0x35,       0x36,         0x37,
        0x38,         0x39,          0x41,        0x42,        0x43,       0x44,       0x45,         0x46,
        0x47,         0x48,          0x49,        0x4A,        0x4B,       0x4C,       0x4D,         0x4E,
        0x4F,         0x50,          0x51,        0x52,        0x53,       0x54,       0x55,         0x56,
        0x57,         0x58,          0x59,        0x5A,        VK_OEM_7,   VK_OEM_5,   VK_OEM_COMMA, VK_OEM_4,
        VK_OEM_MINUS, VK_OEM_PERIOD, VK_OEM_6,    VK_OEM_1,    VK_OEM_2,   VK_BACK,    VK_DELETE,    VK_END,
        VK_EXECUTE,   VK_ESCAPE,     VK_HOME,     VK_INSERT,   VK_PRIOR,   VK_NEXT,    VK_PAUSE,     VK_SPACE,
        VK_TAB,       VK_CAPITAL,    VK_NUMLOCK,  VK_SCROLL,   VK_F1,      VK_F2,      VK_F3,        VK_F4,
        VK_F5,        VK_F6,         VK_F7,       VK_F8,       VK_F9,      VK_F10,     VK_F11,       VK_F12,
        VK_F13,       VK_F14,        VK_F15,      VK_F16,      VK_F17,     VK_F18,     VK_F19,       VK_F20,
        VK_F21,       VK_F22,        VK_F23,      VK_F24,      VK_MENU,    VK_LMENU,   VK_RMENU,     VK_CONTROL,
        VK_LCONTROL,  VK_RCONTROL,   VK_SHIFT,    VK_LSHIFT,   VK_RSHIFT,  VK_LWIN,    VK_RWIN,      VK_SNAPSHOT,
        VK_DOWN,      VK_LEFT,       VK_RIGHT,    VK_UP,       VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2,   VK_NUMPAD3,
        VK_NUMPAD4,   VK_NUMPAD5,    VK_NUMPAD6,  VK_NUMPAD7,  VK_NUMPAD8, VK_NUMPAD9, VK_ADD,       VK_DECIMAL,
        VK_DIVIDE,    VK_OEM_PLUS,   VK_MULTIPLY, VK_SUBTRACT,
    };
  public:
    explicit Win32Window(const char *,const  losSize);
    LRESULT CALLBACK classWndProc(HWND, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        Win32Window *me = reinterpret_cast<Win32Window *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (me)
            return me->classWndProc(hwnd,msg, wParam, lParam);
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    // LibOS
    virtual AbstractWindowPlatform getPlatform() const noexcept final override;
    virtual bool hasWindowClosed() const noexcept final override;
    virtual losResult losUpdateWindow() noexcept final override;
    virtual bool losIsKeyDown(losKeyboardButton) const noexcept final override;
    virtual bool losIsMouseDown(losMouseButton) const noexcept final override;
    virtual losResult losRequestClose() noexcept final override;
    virtual losPosition losRequestMousePosition() const  noexcept final override;
    virtual losPosition losRequestMouseWheelDelta() const noexcept final override;
    virtual losPosition losIsBeingPressed() const noexcept final override;
    virtual void losDestroyWindow() noexcept final override;

    losResult losCreateWindowSurface(AbstractGraphicsContext* handle) noexcept final override;
};