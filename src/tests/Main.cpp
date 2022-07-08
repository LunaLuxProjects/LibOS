#include "Cmake.h"
#include "FileIOTest.h"
#include "NetIOTest.h"
#include <Components/Window.h>
#include <Components/FileIO.h>
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
    losResult res;
    printf("TEST FILEIO API? - [Y/n]:");
    std::string input;
    std::getline(std::cin, input);
    if (input == "Y")
    {
        TEST(testFileIOMain())
    }
    printf("TEST NETIO API? - [Y/n]:");
    std::getline(std::cin, input);
    if (input == "Y")
    {
        TEST(testNetIOMain())
    }
    TEST(refCreateRefractileContext(&handle));
    std::string file CMAKE_SOURCE_DIR;
    file += "/tests";
    TEST(losSetAssetPath(file.c_str()));
    printf("TEST AUDIO API? - [Y/n]:");
    std::getline(std::cin, input);
    if (input == "Y")
    {
        using namespace std::chrono_literals;
        TEST(refAppendAudioContext(handle));
        refAudioBuffer a_buffer;
        refAudioPlayer player;
        TEST(refCreateAudioBuffer(&a_buffer, "$[asset_base]/Audio/piano2.wav"));
        TEST(refPrepPlayer(&player));
        TEST(refPlay(player, a_buffer, 0, 0, 0, 1));
        std::this_thread::sleep_for(2s);
        TEST(refPause(player));
        std::this_thread::sleep_for(2s);
        TEST(refResume(player));
        std::this_thread::sleep_for(2s);
        TEST(refStop(player));
        TEST(refDestroyAudioBuffer(a_buffer));
        TEST(refUnAppendAudioContext(handle));
        printf("TEST AUDIO COMPLETE - Did you hear The Sound?\n");
    }
    printf("TEST Window API? - [Y/n]:");
    std::getline(std::cin, input);
    if (input != "n")
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
        TEST(refAppendGraphicsContext(handle, window, g_info));
        TEST(refCreateCommandBuffer(handle, &buffer));
        refCreateShaderProgramInfo shader_info;
        shader_info.shader_settings = "$[asset_base]/Shader/settings.json";
        shader_info.vertex_shader = "$[asset_base]/Shader/VertexShader.vert.spirv";
        shader_info.fragment_shader = "$[asset_base]/Shader/fragmentShader.frag.spirv";
        shader_info.pre_compiled = true;

        TEST(refCreateShaderProgram(handle, &program, shader_info));
        while (losUpdateWindow(window) != LOS_WINDOW_CLOSE)
        {
            if (losIsKeyDown(window, LOS_KEYBOARD_X))
                losRequestClose(window);
            TEST(refBeginCommands(handle, buffer));
            const float32 colour[4]{0.0f, 0.0f, 0.0f, 0.0f};
            TEST(refCmdBeginDrawing(handle, buffer, nullptr, colour));
            //TEST(refCmdBindShaderProgram(handle, buffer, program));
            //TEST(refCmdDraw(handle, buffer, 3, 1, 0, 0));
            TEST(refCmdEndDrawing(handle, buffer));
            TEST(refEndCommands(handle, buffer));
            TEST(refExecuteCommands(handle, buffer, true));
        }

        TEST(refDestroyCommandBuffer(handle, buffer));
        TEST(refUnAppendGraphicsContext(handle));
        TEST(losDestroyKeyboard(window));
        TEST(losDestroyMouse(window));
        TEST(losDestroyWindow(window));
    }
    TEST(refDestroyRefractileContext(handle));
    return 0;
}