#include "Cmake.h"
#include "FileIOTest.h"
#include "NetIOTest.h"
#include <Components/FileIO.h>
#include <Components/Window.h>
#include <RefractileAPI.h>
#include <chrono>
#include <cstdio>
#include <iostream>
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
    printf("TEST FILEIO API? - [Y/n (default)]:");
    std::string input;
    std::getline(std::cin, input);
    if (input == "Y")
    {
        TEST(testFileIOMain())
    }
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
        size device_count = 0;
        TEST(refAppendAudioContext(handle, &device_count));
        if (device_count == 0)
        {
            printf("%s\n", "AUDIO TEST FAILED: no devices found");
            exit(1);
        }
        size dev_to_used = 0;
        for (size i = 0; i < device_count; i++)
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
    printf("TEST Window API? - [Y/n (default)]:");
    std::getline(std::cin, input);
    if (input == "Y")
    {
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
        refCommandBuffer buffer;
        refShaderProgram program;
        TEST(losCreateWindow(&window, info));
        TEST(losCreateKeyboard(window));
        TEST(losCreateMouse(window));
        refCreateGraphicContextInfo g_info;
        g_info.find_best_device = false;
        /*TEST(refAppendGraphicsContext(handle, window, g_info));
        TEST(refCreateCommandBuffer(handle, &buffer));
        refCreateShaderProgramInfo shader_info;
        shader_info.shader_settings = "$[asset_base]/Shader/settings.json";
        shader_info.vertex_shader = "$[asset_base]/Shader/VertexShader.vert.spirv";
        shader_info.fragment_shader = "$[asset_base]/Shader/fragmentShader.frag.spirv";
        shader_info.pre_compiled = true;

        TEST(refCreateShaderProgram(handle, &program, shader_info));*/
        while (losUpdateWindow(window) != LOS_WINDOW_CLOSE)
        {
            if (losIsKeyDown(window, LOS_KEYBOARD_X))
                losRequestClose(window);
            /*TEST(refBeginCommands(handle, buffer));
            const float32 colour[4]{0.0f, 0.0f, 0.0f, 0.0f};
            TEST(refCmdBeginDrawing(handle, buffer, nullptr, colour));*/
            // TEST(refCmdBindShaderProgram(handle, buffer, program));
            // TEST(refCmdDraw(handle, buffer, 3, 1, 0, 0));
            /*TEST(refCmdEndDrawing(handle, buffer));
            TEST(refEndCommands(handle, buffer));
            TEST(refExecuteCommands(handle, buffer, true));*/
        }

        /*TEST(refDestroyCommandBuffer(handle, buffer));
        TEST(refUnAppendGraphicsContext(handle));*/
        losDestroyKeyboard(window);
        losDestroyMouse(window);
        losDestroyWindow(window);
    }
    refDestroyRefractileContext(handle);
    libOSCleanUp();
    return 0;
}