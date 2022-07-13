#include "../../Cmake.h"
#include "../Headers/StructsDefines.hpp"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "../../Linux/Window/DirectScreen.hpp"
#    include "../../Linux/Window/Xcb.hpp"
#endif
#if CMAKE_SYSTEM_NUMBER == 1
#    include "../../Windows/Window/WinRTWindow.hpp"
#endif
#if CMAKE_SYSTEM_NUMBER == 2
#    include "../../Windows/Window/Win32Window.hpp"
#endif

#include <stdexcept>
losResult losCreateWindow(losWindow *window, losWindowInfo &info)
{
    /* FIXME: this check should stop reusing handles already in use
    if (!(*window))
        return LOS_ERROR_HANDLE_IN_USE;
    */
    (*window) = new losWindow_T();

#if CMAKE_SYSTEM_NUMBER == 0
    if (info.is_compositeServer)
    {
        try
        {
            (*window)->window = new DirectScreen(info.title, info.window_size);
            (*window)->window->setObjectCallback(std::move(info.request_callback));
        }
        catch (const std::exception &e)
        {
            info.is_compositeServer = false;
            return losCreateWindow(window, info);
        }
        return LOS_SUCCESS;
    }

    try
    {
        (*window)->window = new WaylandWindow(info.title, info.window_size);
        (*window)->window->setObjectCallback(std::move(info.request_callback));
    }
    catch (const std::exception &)
    {
        try
        {
            (*window)->window = new XcbWindow(info.title, info.window_size);
            (*window)->window->setObjectCallback(std::move(info.request_callback));
            printf("LibOS - Window Info: %s\n", "WaylandWindow failed to initialize failing back to XcbWindow");
        }
        catch (const std::exception &e)
        {
            printf("LibOS - Window Error: %s\n", e.what());
            return LOS_ERROR_COULD_NOT_INIT;
        }
    }
#endif
#if CMAKE_SYSTEM_NUMBER == 1
    try
    {
        (*window)->window = new WinRTWindow(info.title, info.window_size);
        (*window)->window->setObjectCallback(std::move(info.request_callback));
    }
    catch (const std::exception &)
    {
        return LOS_ERROR_COULD_NOT_INIT;
    }
#endif
#if CMAKE_SYSTEM_NUMBER == 2
    try
    {
        (*window)->window = new Win32Window(info.title, info.window_size);
        (*window)->window->setObjectCallback(std::move(info.request_callback));
    }
    catch (const std::exception &)
    {
        return LOS_ERROR_COULD_NOT_INIT;
    }
#endif
    return LOS_SUCCESS;
}

losResult losCreateKeyboard(losWindow window)
{
    return window->window->losCreateKeyboard();
}

losResult losCreateMouse(losWindow window)
{
    return window->window->losCreateMouse();
}

losResult losCreateTouch(losWindow window)
{
    return window->window->losCreateTouch();
}

losResult losUpdateWindow(losWindow window) noexcept
{
    return window->window->losUpdateWindow();
}

bool losIsKeyDown(losWindow window, losKeyboardButton key) noexcept
{
    return window->window->losIsKeyDown(key);
}

bool losIsMouseDown(losWindow window, losMouseButton button) noexcept
{
    return window->window->losIsMouseDown(button);
}

losResult losRequestClose(losWindow window) noexcept
{
    return window->window->losRequestClose();
}

losPosition losRequestMousePosition(losWindow window) noexcept
{
    return window->window->losRequestMousePosition();
}

losPosition losRequestMouseWheelDelta(losWindow window) noexcept
{
    return window->window->losRequestMouseWheelDelta();
}

losPosition losIsBeingPressed(losWindow window) noexcept
{
    return window->window->losIsBeingPressed();
}

void losDestroyKeyboard(losWindow window) noexcept
{
    window->window->losDestroyKeyboard();
}

void losDestroyMouse(losWindow window) noexcept
{
    window->window->losDestroyMouse();
}

void losDestroyTouch(losWindow window) noexcept
{
    window->window->losDestroyTouch();
}

void losDestroyWindow(losWindow window) noexcept
{
    window->window->losDestroyWindow();
    delete window->window;
}