#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#    include "../../Interface/Headers/AbstractGraphicsContex.hpp"
#    include "../Graphics/VkComponents/CVkError.hpp"
#    include "../Graphics/Vulkan.hpp"
#    include "../Graphics/VkComponents/CVkInstance.hpp"
#    include "Wayland.hpp"
#    include <vulkan/vulkan_core.h>
#    include <vulkan/vulkan_wayland.h>
#    include <wayland-client.h>

/*

const xdg_surface_listener surface_listener = {[](void *, struct xdg_surface *, uint32_t) {}};

const xdg_toplevel_listener toplevel_listener = {
    [](void *, xdg_toplevel *, int32_t, int32_t, struct wl_array *) {},
    [](void *data, xdg_toplevel *) { losRequestClose(static_cast<losWindow>(data)); },
    [](void *, xdg_toplevel *, int32_t, int32_t) {}};

const wl_keyboard_listener keyboard_listener = {
    [](void *, wl_keyboard *, uint32_t, int, uint32_t) {},
    [](void *, wl_keyboard *, uint32_t, wl_surface *, wl_array *) {},
    [](void *, wl_keyboard *, uint32_t, wl_surface *) {},
    [](void *, wl_keyboard *, uint32_t, uint32_t, uint32_t, uint32_t) {},
    [](void *, wl_keyboard *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t) {},
    [](void *, wl_keyboard *, int32_t, int32_t) {}};

const wl_pointer_listener pointer_listener = {
    [](void *, wl_pointer *, uint32_t, wl_surface *, wl_fixed_t, wl_fixed_t) {},
    [](void *, wl_pointer *, uint32_t, wl_surface *) {},
    [](void *, wl_pointer *, uint32_t, wl_fixed_t, wl_fixed_t) {},
    [](void *, wl_pointer *, uint32_t, uint32_t, uint32_t, uint32_t) {},
    [](void *, wl_pointer *, uint32_t, uint32_t, wl_fixed_t) {},
    [](void *, wl_pointer *) {},
    [](void *, wl_pointer *, uint32_t) {},
    [](void *, wl_pointer *, uint32_t, uint32_t) {},
    [](void *, wl_pointer *, uint32_t, int32_t) {}};

const wl_seat_listener seat_listener = {[](void *data, wl_seat *seat, uint32_t caps) {
                                            losWindow win = (losWindow)data;
                                            if ((caps & WL_SEAT_CAPABILITY_POINTER) && !win->pointer)
                                            {
                                                win->pointer = wl_seat_get_pointer(seat);
                                                wl_pointer_add_listener(win->pointer, &pointer_listener, win);
                                            }
                                            else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && win->pointer)
                                            {
                                                wl_pointer_destroy(win->pointer);
                                                win->pointer = NULL;
                                            }

                                            if (caps & WL_SEAT_CAPABILITY_KEYBOARD)
                                            {
                                                win->keyboard = wl_seat_get_keyboard(seat);
                                                wl_keyboard_add_listener(win->keyboard, &keyboard_listener, win);
                                            }
                                            else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD))
                                            {
                                                wl_keyboard_destroy(win->keyboard);
                                                win->keyboard = NULL;
                                            }
                                        },
                                        [](void *, wl_seat *, const char *) {}};

static const xdg_wm_base_listener xdg_wm_base_listener = {
    [](void *, struct xdg_wm_base *xdg_wm_base, uint32_t serial) { xdg_wm_base_pong(xdg_wm_base, serial); }};

const wl_registry_listener registry_listener = {
    [](void *data, wl_registry *registry, uint32_t id, const char *interface, uint32_t) {
        losWindow win = (losWindow)data;

        if (strcmp(interface, "wl_compositor") == 0)
        {
            win->compositor = (wl_compositor *)wl_registry_bind(registry, id, &wl_compositor_interface, 1);
        }
        else if (strcmp(interface, "xdg_wm_base") == 0)
        {
            win->xdg_shell = (xdg_wm_base *)wl_registry_bind(registry, id, &xdg_wm_base_interface, 3);
            xdg_wm_base_add_listener(win->xdg_shell, &xdg_wm_base_listener, win);
        }
        else if (strcmp(interface, "wl_seat") == 0)
        {
            win->seat = (wl_seat *)wl_registry_bind(registry, id, &wl_seat_interface, 1);
            wl_seat_add_listener(win->seat, &seat_listener, win);
        }
    },
    [](void *, wl_registry *, uint32_t) {}};
*/

AbstractWindowPlatform WaylandWindow::getPlatform() const noexcept
{
    return AbstractWindowPlatform::WAYLAND_WINDOW;
};

WaylandWindow::WaylandWindow(const char *title, losSize win_size)
{
    display = wl_display_connect(NULL);
    if (!display)
        throw std::runtime_error("could not connect to wayland");

    registry = wl_display_get_registry(display);
    if (!registry)
        throw std::runtime_error("could not connect to wayland");

    // wl_registry_add_listener(registry, &registry_listener, this);
    wl_display_roundtrip(display);

    if (!compositor)
        throw std::runtime_error("could not connect to wayland");

    if (!xdg_shell)
        throw std::runtime_error("could not connect to wayland");

    surface = wl_compositor_create_surface(compositor);
    if (!surface)
        throw std::runtime_error("could not connect to wayland");

    xdg_win_surface = xdg_wm_base_get_xdg_surface(xdg_shell, surface);
    if (!xdg_win_surface)
        throw std::runtime_error("could not connect to wayland");

    // xdg_surface_add_listener(xdg_win_surface, &surface_listener, this);

    xdg_top_level = xdg_surface_get_toplevel(xdg_win_surface);
    // xdg_toplevel_add_listener(xdg_top_level,&toplevel_listener, this);
    xdg_toplevel_set_app_id(xdg_top_level, "LibOSWindowClass");
    xdg_toplevel_set_title(xdg_top_level, title);
    wl_surface_commit(surface);

    wl_region *region = wl_compositor_create_region(compositor);
    wl_region_add(region, 0, 0, win_size.width, win_size.height);
    wl_surface_set_opaque_region(surface, region);
    wl_region_destroy(region);

    wl_display_flush(display);
}
losResult WaylandWindow::losCreateKeyboard() noexcept
{
    return LOS_SUCCESS;
}
losResult WaylandWindow::losCreateMouse() noexcept
{
    return LOS_SUCCESS;
}

bool WaylandWindow::hasWindowClosed() const noexcept
{
    return false;
}
losResult WaylandWindow::losUpdateWindow() noexcept
{
    if (wl_display_dispatch(display) == -1)
        return LOS_WINDOW_CLOSE;
    return LOS_SUCCESS;
}
bool WaylandWindow::losIsKeyDown(losKeyboardButton) const noexcept
{
    return false;
}
bool WaylandWindow::losIsMouseDown(losMouseButton) const noexcept
{
    return false;
}
losResult WaylandWindow::losRequestClose() noexcept
{
    return LOS_SUCCESS;
};
losPosition WaylandWindow::losRequestMousePosition() const noexcept
{
    return {0, 0};
}
losPosition WaylandWindow::losRequestMouseWheelDelta() const noexcept
{
    return {0, 0};
}
losPosition WaylandWindow::losIsBeingPressed() const noexcept
{
    return {0, 0};
}

void WaylandWindow::losDestroyKeyboard() noexcept
{
    if (keyboard)
        wl_keyboard_destroy(keyboard);
}

void WaylandWindow::losDestroyMouse() noexcept
{
    if (pointer)
        wl_pointer_destroy(pointer);
    if (seat)
        wl_seat_destroy(seat);
}

void WaylandWindow::losDestroyWindow() noexcept
{
    if (xdg_win_surface)
        xdg_surface_destroy(xdg_win_surface);
    if (surface)
        wl_surface_destroy(surface);
    if (xdg_shell)
        xdg_wm_base_destroy(xdg_shell);
    if (compositor)
        wl_compositor_destroy(compositor);
    if (registry)
        wl_registry_destroy(registry);
    if (display)
        wl_display_disconnect(display);
}
losResult WaylandWindow::losCreateWindowSurface(AbstractGraphicsContext *handle) noexcept
{
    VulkanContext *context = static_cast<VulkanContext *>(handle);
    VkResult result;
    const VkWaylandSurfaceCreateInfoKHR surface_info{VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR, nullptr, 0,
                                                     display, surface};
    [[unlikely]] if (handle->check((result = vkCreateWaylandSurfaceKHR(~(*context->getInst()), &surface_info, nullptr,
                                                                       context->setSurf()))))
    {
        printf("LIB OS: Vulkan Error: %s\n",std::to_string(vkresult_translation_table.find(result)).c_str()); 
        return LOS_ERROR_COULD_NOT_INIT;
    }
    return LOS_SUCCESS;
}

/* Generated by wayland-scanner 1.20.0 */

/*
 * Copyright © 2008-2013 Kristian Høgsberg
 * Copyright © 2013      Rafael Antognolli
 * Copyright © 2013      Jasper St. Pierre
 * Copyright © 2010-2013 Intel Corporation
 * Copyright © 2015-2017 Samsung Electronics Co., Ltd
 * Copyright © 2015-2017 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#    include <wayland-util.h>

#    ifndef __has_attribute
#        define __has_attribute(x) 0 /* Compatibility with non-clang compilers. */
#    endif

#    define WL_PRIVATE

extern const struct wl_interface wl_output_interface;
extern const struct wl_interface wl_seat_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface xdg_popup_interface;
extern const struct wl_interface xdg_positioner_interface;
extern const struct wl_interface xdg_surface_interface;
extern const struct wl_interface xdg_toplevel_interface;

static const struct wl_interface *xdg_shell_types[] = {
    NULL,
    NULL,
    NULL,
    NULL,
    &xdg_positioner_interface,
    &xdg_surface_interface,
    &wl_surface_interface,
    &xdg_toplevel_interface,
    &xdg_popup_interface,
    &xdg_surface_interface,
    &xdg_positioner_interface,
    &xdg_toplevel_interface,
    &wl_seat_interface,
    NULL,
    NULL,
    NULL,
    &wl_seat_interface,
    NULL,
    &wl_seat_interface,
    NULL,
    NULL,
    &wl_output_interface,
    &wl_seat_interface,
    NULL,
    &xdg_positioner_interface,
    NULL,
};

static const struct wl_message xdg_wm_base_requests[] = {
    {"destroy", "", xdg_shell_types + 0},
    {"create_positioner", "n", xdg_shell_types + 4},
    {"get_xdg_surface", "no", xdg_shell_types + 5},
    {"pong", "u", xdg_shell_types + 0},
};

static const struct wl_message xdg_wm_base_events[] = {
    {"ping", "u", xdg_shell_types + 0},
};

WL_PRIVATE const struct wl_interface xdg_wm_base_interface = {
    "xdg_wm_base", 4, 4, xdg_wm_base_requests, 1, xdg_wm_base_events,
};

static const struct wl_message xdg_positioner_requests[] = {
    {"destroy", "", xdg_shell_types + 0},
    {"set_size", "ii", xdg_shell_types + 0},
    {"set_anchor_rect", "iiii", xdg_shell_types + 0},
    {"set_anchor", "u", xdg_shell_types + 0},
    {"set_gravity", "u", xdg_shell_types + 0},
    {"set_constraint_adjustment", "u", xdg_shell_types + 0},
    {"set_offset", "ii", xdg_shell_types + 0},
    {"set_reactive", "3", xdg_shell_types + 0},
    {"set_parent_size", "3ii", xdg_shell_types + 0},
    {"set_parent_configure", "3u", xdg_shell_types + 0},
};

WL_PRIVATE const struct wl_interface xdg_positioner_interface = {
    "xdg_positioner", 4, 10, xdg_positioner_requests, 0, NULL,
};

static const struct wl_message xdg_surface_requests[] = {
    {"destroy", "", xdg_shell_types + 0},        {"get_toplevel", "n", xdg_shell_types + 7},
    {"get_popup", "n?oo", xdg_shell_types + 8},  {"set_window_geometry", "iiii", xdg_shell_types + 0},
    {"ack_configure", "u", xdg_shell_types + 0},
};

static const struct wl_message xdg_surface_events[] = {
    {"configure", "u", xdg_shell_types + 0},
};

WL_PRIVATE const struct wl_interface xdg_surface_interface = {
    "xdg_surface", 4, 5, xdg_surface_requests, 1, xdg_surface_events,
};

static const struct wl_message xdg_toplevel_requests[] = {
    {"destroy", "", xdg_shell_types + 0},
    {"set_parent", "?o", xdg_shell_types + 11},
    {"set_title", "s", xdg_shell_types + 0},
    {"set_app_id", "s", xdg_shell_types + 0},
    {"show_window_menu", "ouii", xdg_shell_types + 12},
    {"move", "ou", xdg_shell_types + 16},
    {"resize", "ouu", xdg_shell_types + 18},
    {"set_max_size", "ii", xdg_shell_types + 0},
    {"set_min_size", "ii", xdg_shell_types + 0},
    {"set_maximized", "", xdg_shell_types + 0},
    {"unset_maximized", "", xdg_shell_types + 0},
    {"set_fullscreen", "?o", xdg_shell_types + 21},
    {"unset_fullscreen", "", xdg_shell_types + 0},
    {"set_minimized", "", xdg_shell_types + 0},
};

static const struct wl_message xdg_toplevel_events[] = {
    {"configure", "iia", xdg_shell_types + 0},
    {"close", "", xdg_shell_types + 0},
    {"configure_bounds", "4ii", xdg_shell_types + 0},
};

WL_PRIVATE const struct wl_interface xdg_toplevel_interface = {
    "xdg_toplevel", 4, 14, xdg_toplevel_requests, 3, xdg_toplevel_events,
};

static const struct wl_message xdg_popup_requests[] = {
    {"destroy", "", xdg_shell_types + 0},
    {"grab", "ou", xdg_shell_types + 22},
    {"reposition", "3ou", xdg_shell_types + 24},
};

static const struct wl_message xdg_popup_events[] = {
    {"configure", "iiii", xdg_shell_types + 0},
    {"popup_done", "", xdg_shell_types + 0},
    {"repositioned", "3u", xdg_shell_types + 0},
};

WL_PRIVATE const struct wl_interface xdg_popup_interface = {
    "xdg_popup", 4, 3, xdg_popup_requests, 3, xdg_popup_events,
};
#endif