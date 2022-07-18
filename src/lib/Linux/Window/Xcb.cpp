#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "../../Interface/Headers/AbstractGraphicsContex.hpp"
#    include "../Graphics/VkComponents/CVkError.hpp"
#    include "../Graphics/VkComponents/CVkInstance.hpp"
#    include "../Graphics/Vulkan.hpp"
#    include "Xcb.hpp"
#    include <lstd/ClibLink.h>
#    include <vulkan/vulkan_core.h>
#    include <vulkan/vulkan_xcb.h>
#    include <xcb/xcb.h>

AbstractWindowPlatform XcbWindow::getPlatform() const noexcept
{
    return AbstractWindowPlatform::XCB_LIB_WINDOW;
}

XcbWindow::XcbWindow(const char *title, losSize win_size)
{
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    con = xcb_connect(nullptr, nullptr);
    screen = xcb_setup_roots_iterator(xcb_get_setup(con)).data;
    uint32_t values[2];
    values[0] = screen->white_pixel;
    values[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
                XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
                XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW;

    win = xcb_generate_id(con);

    xcb_create_window(con, XCB_COPY_FROM_PARENT, win, screen->root, 0, 0, win_size.width, win_size.height, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, mask, values);

    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(con, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(con, cookie, nullptr);

    xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(con, 0, 16, "WM_DELETE_WINDOW");
    atom_wm_delete_window = xcb_intern_atom_reply(con, cookie2, nullptr);

    xcb_change_property(con, XCB_PROP_MODE_REPLACE, win, (*reply).atom, 4, 32, 1, &(*atom_wm_delete_window).atom);
    free(reply);

    xcb_change_property(con, XCB_PROP_MODE_REPLACE, win, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, std::strlen(title),
                        title);

    /* Map the window on the screen */
    xcb_map_window(con, win);
    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(con)).data;

    const uint32_t coords[] = {(static_cast<uint32_t>(screen->width_in_pixels) / 3),
                               (static_cast<uint32_t>(screen->height_in_pixels) / 4)};
    xcb_configure_window(con, win, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, coords);
    /* Make sure commands are sent before we pause, so window is shown */
    xcb_flush(con);
}

bool XcbWindow::hasWindowClosed() const noexcept
{
    return shouldClose;
}

losResult XcbWindow::losUpdateWindow() noexcept
{
    xcb_generic_event_t *event = xcb_poll_for_event(con);
    if (event == nullptr)
        return LOS_SUCCESS;

    if (!isInWindow)
    {
        if (!isRest)
        {
            for (uint8_t i = 0; i < 255; i++)
                keys[i] = false;

            for (uint8_t i = 0; i < 2; i++)
                buttons[i] = false;
            isRest = true;
        }
    }

    switch (event->response_type & ~0x80)
    {
    case XCB_KEY_PRESS: {
        if (isInWindow)
        {
            auto key = reinterpret_cast<xcb_key_press_event_t *>(event);
            keys[key->detail] = true;
        }
    }
    break;
    case XCB_KEY_RELEASE: {
        if (isInWindow)
        {
            auto key = reinterpret_cast<xcb_key_release_event_t *>(event);
            keys[key->detail] = false;
        }
    }
    break;
    case XCB_BUTTON_PRESS: {
        if (isInWindow)
        {
            auto button = reinterpret_cast<xcb_button_press_event_t *>(event);
            buttons[button->detail] = true;
        }
    }
    break;
    case XCB_BUTTON_RELEASE: {
        if (isInWindow)
        {
            auto button = reinterpret_cast<xcb_button_release_event_t *>(event);
            buttons[button->detail] = false;
        }
    }
    break;
    case XCB_MOTION_NOTIFY: {
        if (isInWindow)
        {
            auto mouse_move = reinterpret_cast<xcb_motion_notify_event_t *>(event);
            x = static_cast<uint16>(mouse_move->event_x);
            y = static_cast<uint16>(mouse_move->event_y);
        }
    }
    break;
    case XCB_ENTER_NOTIFY: {
        isInWindow = true;
        isRest = false;
    }
    break;
    case XCB_LEAVE_NOTIFY: {
        isInWindow = false;
    }
    break;
    case XCB_CLIENT_MESSAGE:
        if ((*(xcb_client_message_event_t *)event).data.data32[0] == (*atom_wm_delete_window).atom)
        {
            shouldClose = true;
            return LOS_WINDOW_CLOSE;
        }
        break;
    default:
        break;
    }
    free(event);
    return LOS_SUCCESS;
}

bool XcbWindow::losIsKeyDown(losKeyboardButton button) const noexcept
{
    return keys[button];
}

bool XcbWindow::losIsMouseDown(losMouseButton button) const noexcept
{
    return buttons[button];
}

losResult XcbWindow::losRequestClose() noexcept
{
    shouldClose = true;
    losDestroyWindow();
    return LOS_SUCCESS;
}

losPosition XcbWindow::losRequestMousePosition() const noexcept
{
    return {x, y};
}

losPosition XcbWindow::losRequestMouseWheelDelta() const noexcept
{
    return {0, 0};
}

losPosition XcbWindow::losIsBeingPressed() const noexcept
{
    return {x, y};
}

void XcbWindow::losDestroyWindow() noexcept
{
    xcb_destroy_window(con, win);
    xcb_disconnect(con);
    free(atom_wm_delete_window);
}

losResult XcbWindow::losCreateWindowSurface(AbstractGraphicsContext *handle) noexcept
{
    VulkanContext *context = static_cast<VulkanContext *>(handle);
    VkResult result;
    const VkXcbSurfaceCreateInfoKHR surface_info{VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR, nullptr, 0, con, win};
    [[unlikely]] if (handle->check((result = vkCreateXcbSurfaceKHR(~(*context->getInst()), &surface_info, nullptr,
                                                                   context->setSurf()))))
    {
        printf("LIB OS: Vulkan Error: %s\n", std::to_string(vkresult_translation_table.find(result)).c_str());
        return LOS_ERROR_COULD_NOT_INIT;
    }
    return LOS_SUCCESS;
}
#endif