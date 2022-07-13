#include "../../Interface/Headers/AbstractWindow.hpp"
#define WINRT_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <winrt/base.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>

class WinRTWindow : public AbstractWindow
{
    // UWP
    struct WinRTWindowInternal : winrt::implements<WinRTWindowInternal, winrt::Windows::ApplicationModel::Core::IFrameworkViewSource, winrt::Windows::ApplicationModel::Core::IFrameworkView>
    {
        std::atomic<WinRTWindow *> parent;
        const char *title;
        const losSize win_size;
        WinRTWindowInternal(WinRTWindow *,const char *,const losSize );
        winrt::Windows::ApplicationModel::Core::IFrameworkView CreateView();
        void Initialize(const winrt::Windows::ApplicationModel::Core::CoreApplicationView&);
        void Load(const winrt::hstring &);
        void Uninitialize();
        void Run();
        void SetWindow(const winrt::Windows::UI::Core::CoreWindow &);
    };

  public:
    explicit WinRTWindow(const char *, losSize){};
    // LibOS
    virtual AbstractWindowPlatform getPlatform() const noexcept final override;
    virtual losResult losCreateKeyboard() noexcept final override
    {
        return LOS_SUCCESS;
    };
    virtual losResult losCreateMouse() noexcept final override
    {
        return LOS_SUCCESS;
    };
    virtual losResult losCreateTouch() noexcept final override
    {
        return LOS_SUCCESS;
    };
    virtual bool hasWindowClosed() const noexcept final override;
    virtual losResult losUpdateWindow() noexcept final override;
    virtual bool losIsKeyDown(losKeyboardButton) const noexcept final override;
    virtual bool losIsMouseDown(losMouseButton) const noexcept final override;
    virtual losResult losRequestClose() noexcept final override;
    virtual losPosition losRequestMousePosition() noexcept final override;
    virtual losPosition losRequestMouseWheelDelta() noexcept final override;
    virtual losPosition losIsBeingPressed() const noexcept final override;
    virtual losResult losDestroyKeyboard() noexcept final override
    {
        return LOS_SUCCESS;
    };
    virtual losResult losDestroyMouse() noexcept final override
    {
        return LOS_SUCCESS;
    };
    virtual losResult losDestroyTouch() noexcept final override
    {
        return LOS_SUCCESS;
    };
    virtual losResult losDestroyWindow() noexcept final override;

    losResult losCreateWindowSurface(refHandle handle) noexcept final override;
};