#include "../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#include "Window.hpp"
#include "Window/Wayland.hpp"
#include "Window/Xcb.hpp"
#include "Window/DirectScreen.hpp"
#include <Components/Defines.h>
#include <Components/Window.h>
#include <stdexcept>

losResult losCreateWindow(losWindow *window, losWindowInfo &info)
{
    /* FIXME: this check should stop reusing handles already in use
    if (!(*window))
        return LOS_ERROR_HANDLE_IN_USE;
    */
    (*window) =  new losWindow_T();

    if(info.is_compositeServer)
    {
        try
        {
            (*window)->window = new DirectScreen(info.title, info.window_size);
        }
        catch(const std::exception& e)
        {
            info.is_compositeServer = false;
            return losCreateWindow(window, info);
        }
        return LOS_SUCCESS;
    }

    try
    {
        (*window)->window = new WaylandWindow(info.title, info.window_size);
    }
    catch(const std::exception&)
    {
        try
        {
            (*window)->window = new XcbWindow(info.title, info.window_size);
            printf("LibOS - Window Info: %s\n", "WaylandWindow failed to initialize failing back to XcbWindow");
        }
        catch(const std::exception& e)
        {
            printf("LibOS - Window Error: %s\n", e.what());
            return LOS_ERROR_COULD_NOT_INIT;
        }
        
    }
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

losResult losUpdateWindow(losWindow window)
{
    return window->window->losUpdateWindow();
}

bool losIsKeyDown(losWindow window, losKeyboardButton key)
{
    return window->window->losIsKeyDown(key);
}

bool losIsMouseDown(losWindow window, losMouseButton button)
{
    return window->window->losIsMouseDown(button);
}

losResult losRequestClose(losWindow window)
{
    return window->window->losRequestClose();
}

losPosition losRequestMousePosition(losWindow window)
{
    return window->window->losRequestMousePosition();
}

losPosition losRequestMouseWheelDelta(losWindow window)
{
    return window->window->losRequestMouseWheelDelta();
}

losPosition losIsBeingPressed(losWindow window)
{
    return window->window->losIsBeingPressed();
}

losResult losDestroyKeyboard(losWindow window)
{
    return window->window->losDestroyKeyboard();
}

losResult losDestroyMouse(losWindow window)
{
    return window->window->losDestroyMouse();
}

losResult losDestroyTouch(losWindow window)
{
    return window->window->losDestroyTouch();
}

losResult losDestroyWindow(losWindow window)
{
    losResult ret = window->window->losDestroyWindow();
    delete window->window;
    return ret;
}
#endif