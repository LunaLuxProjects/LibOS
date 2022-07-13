#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 1
#    include "WinRTWindow.hpp"
#    pragma comment(lib, "windowsapp")
#    include <future>

WinRTWindow::WinRTWindowInternal::WinRTWindowInternal(WinRTWindow *x, const char *title_, const losSize win_size_)
    : parent(x)
    , title(title_)
    , win_size(win_size_)
{
}

winrt::Windows::ApplicationModel::Core::IFrameworkView WinRTWindow::WinRTWindowInternal::CreateView()
{
    return *this;
}

void WinRTWindow::WinRTWindowInternal::Initialize(const winrt::Windows::ApplicationModel::Core::CoreApplicationView &)
{
}

void WinRTWindow::WinRTWindowInternal::Load(const winrt::hstring &)
{

}

void WinRTWindow::WinRTWindowInternal::Uninitialize()
{
}

void WinRTWindow::WinRTWindowInternal::Run()
{
    const auto window = winrt::Windows::UI::Core::CoreWindow::GetForCurrentThread();
    window.Activate();

    winrt::Windows::UI::Core::CoreDispatcher const dispatcher = window.Dispatcher();
    dispatcher.ProcessEvents(winrt::Windows::UI::Core::CoreProcessEventsOption::ProcessUntilQuit);
}

void WinRTWindow::WinRTWindowInternal::SetWindow(const winrt::Windows::UI::Core::CoreWindow &)
{
}

AbstractWindowPlatform WinRTWindow::getPlatform() const noexcept
{
    return AbstractWindowPlatform::WIN_RT_WINDOW;
}

bool WinRTWindow::hasWindowClosed() const noexcept
{
    return false;
}

losResult WinRTWindow::losUpdateWindow() noexcept
{
    return LOS_SUCCESS;
}

bool WinRTWindow::losIsKeyDown(losKeyboardButton) const noexcept
{
    return false;
}

bool WinRTWindow::losIsMouseDown(losMouseButton) const noexcept
{
    return false;
}

losResult WinRTWindow::losRequestClose() noexcept
{
    losDestroyWindow();
    return LOS_SUCCESS;
}

losPosition WinRTWindow::losRequestMousePosition() noexcept
{
    return {0, 0};
}

losPosition WinRTWindow::losRequestMouseWheelDelta() noexcept
{
    return {0, 0};
}

losPosition WinRTWindow::losIsBeingPressed() const noexcept
{
    return {0, 0};
}

losResult WinRTWindow::losDestroyWindow() noexcept
{
    return LOS_SUCCESS;
}

losResult WinRTWindow::losCreateWindowSurface(refHandle) noexcept
{
    return LOS_SUCCESS;
}
#endif
