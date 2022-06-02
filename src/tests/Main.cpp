#include "Components/Defines.h"
#include "FileIOTest.h"
#include "NetIOTest.h"
#include "RefractileAPI.h"
#include <CrystalOS.h>
#include <cstdio>
#include <string>
#define TEST(func)                                                \
    if ((res = func) != LOS_SUCCESS)                              \
    {                                                             \
        printf("%s: %x\n", "the tests failed with ERROR", res); \
        exit(1); \
    }

refHandle handle;

int main()
{
    losResult res;
    refCommandBuffer buffer;
    refShaderProgram program;
    //TEST(testFileIOMain())
    //TEST(testNetIOMain())
    printf("final test  window test\n");
    losWindowInfo info;
    info.sub_window = false;
    std::string name = "this is the test window";
    info.title = name.data();
    info.title_size = name.size();
    info.window_size.width = 720;
    info.window_size.height = 1280;
    info.request_callback = [](const char * object)
    { 
        if(std::string(object) == "refHandle") 
            return (void*)handle; 
        
        return (void*)nullptr;
    };

    losWindow window;
    TEST(losCreateWindow(&window, info));
    TEST(losCreateKeyboard(window));
    TEST(losCreateMouse(window));
    TEST(refCreateRefractileContext(&handle));
    TEST(refAppendAudioContext(handle));

    refAudioDevice* devices_list;
    uint8 device_count;
    TEST(refGetAudioDeviceList(handle, devices_list,device_count));
    TEST(refSetAudioDevice(handle,devices_list[0]));


#ifndef __linux__
    TEST(refAppendGraphicsContext(handle,window));

    TEST(refCreateCommandBuffer(handle,&buffer));
    refCreateShaderProgramInfo shader_info;
    shader_info.shaderLayout = "$[binary_base]/layout.json";
    shader_info.vertexShader = "$[binary_base]/vertexShader.gim";
    shader_info.fragmentShader = "$[binary_base]/fragmentShader.gim";

    TEST(refCreateShaderProgram(handle,&program,shader_info));

    TEST(refBeginCommands(handle,buffer));
    TEST(refBindShaderProgram(handle,buffer,program));
    TEST(refEndCommands(handle,buffer));

    while (losUpdateWindow(window) != LOS_WINDOW_CLOSE)
#endif
    {
        if (losIsKeyDown(window, LOS_KEYBOARD_X))
            losRequestClose(window);
#ifndef __linux__
        TEST(refExecuteCommands(handle,buffer,true));
#endif
    }

#ifndef __linux__
    TEST(refDestroyShaderProgram(handle,program));
    TEST(refDestroyCommandBuffer(handle,buffer));
    TEST(refUnAppendGraphicsContext(handle));
#endif
    TEST(refUnAppendAudioContext(handle));
    TEST(refDestroyRefractileContext(handle));
    TEST(losDestroyKeyboard(window));
    TEST(losDestroyMouse(window));
    TEST(losDestroyWindow(window));

    return 0;
}