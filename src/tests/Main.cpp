#include "Cmake.h"
#include "FileIOTest.h"
#include "NetIOTest.h"
#include <chrono>
#include <cstdio>
#include <iostream>
#include <libos/FileIO.h>
#include <libos/RefractileAPI.h>
#include <libos/Window.h>
#include <string>
#include <thread>
#define TEST(func)                                              \
    if ((res = func) != LOS_SUCCESS)                            \
    {                                                           \
        printf("%s: %x\n", "the tests failed with ERROR", res); \
        exit(1);                                                \
    }

refHandle handle;

int main()
{
    libOSInit();
    losResult res;
    std::string input;
    //TEST(testFileIOMain())
    printf("TEST NETIO API? - [Y/n (default)]:");
    std::getline(std::cin, input);
    if (input == "Y")
    {
        TEST(testNetIOMain())
    }
    TEST(refCreateRefractileContext(&handle));
    std::string file CMAKE_SOURCE_DIR;
    file += "/tests";
    TEST(losSetAssetPath(file.c_str()));
    printf("TEST AUDIO API? - [Y/n (default)]:");
    std::getline(std::cin, input);
    if (input == "Y")
    {
        refAudioDevice dev;
        data_size device_count = 0;
        TEST(refAppendAudioContext(handle, &device_count));
        if (device_count == 0)
        {
            printf("%s\n", "AUDIO TEST FAILED: no devices found");
            exit(1);
        }
        data_size dev_to_used = 0;
        for (data_size i = 0; i < device_count; i++)
        {
            refAudioDeviceProperties prop{};
            TEST(refGetAudioDeviceProperties(i, &prop));
            printf("Name: %s\n Channels: %x\n  Device Type: %x\n\n", prop.name, prop.audio_channels,
                   prop.audio_exe_type);
            if (prop.audio_exe_type == REF_EXE_TYPE_OUTPUT)
                dev_to_used = i;
        }
        TEST(refRetreveSystemAudioDevice(&dev, dev_to_used));

        refReleaseSystemAudioDevice(dev);
        refUnAppendAudioContext(handle);

        printf("TEST AUDIO COMPLETE - Did you hear The Sound?\n");
    }
    printf("final test  window test\n");
    losWindowInfo info;
    std::string name = "this is the test window";
    info.title = name.data();
    info.title_size = name.size();
    info.window_size.width = 1280;
    info.window_size.height = 720;
    info.request_callback = [](const char *object) {
        if (std::string(object) == "refHandle")
            return (void *)handle;

        return (void *)nullptr;
    };
    losWindow window;
    refGraphicsDevice device;
    data_size dev_count = 0;
    TEST(refAppendGraphicsContext(handle, &dev_count));
    data_size dev_to_used = 0;
    for (data_size i = 0; i < dev_count; i++)
    {
        refGraphicsDeviceProperties prop{};
        TEST(refGetGraphicsDeviceProperties(i, &prop));
        printf("Name: %s\n Device Type: %x\n\n", prop.name, prop.dev_type);
        if (prop.dev_type == REF_DEV_TYPE_DISCRETE_GPU)
            dev_to_used = i;
    }
    TEST(losCreateWindow(&window, info));
    TEST(refRetreveSystemGraphicsDevice(&device, window, dev_to_used));
    TEST(losCreateKeyboard(window));
    TEST(losCreateMouse(window));

    while (losUpdateWindow(window) != LOS_WINDOW_CLOSE)
    {
        if (losIsKeyDown(window, LOS_KEYBOARD_X))
            losRequestClose(window);
    }

    refReleaseSystemGraphicsDevice(device);
    refUnAppendGraphicsContext(handle);
    losDestroyKeyboard(window);
    losDestroyMouse(window);
    losDestroyWindow(window);
    refDestroyRefractileContext(handle);
    libOSCleanUp();
    return 0;
}