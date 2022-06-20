#include <RefractileAPI.h>
#include <Components/FileIO.h>
#include "Structs.h"
#include <string>
#include <vector>

losResult readFile(const std::string path,void** data,size* d_size)
{
    losResult res;
    losFileHandle fileHandle;
    losFileOpenInfo file;                                  
    file.fileBits = LOS_FILE_BIT_READ;                             
    file.path = path.data();                                      
    file.path_size = path.size();
    if ((res = losOpenFile(&fileHandle, file)) != LOS_SUCCESS) 
        return res;                                        
    if ((res = losReadFile(fileHandle,data,d_size)) != LOS_SUCCESS)          
        return res;                                        
    if ((res = losCloseFile(fileHandle)) != LOS_SUCCESS)       
        return res;

    return LOS_SUCCESS;
}


losResult createShaderModule(refHandle handle,VkResult* result,VkShaderModule* module,const char* path,const bool compiled) 
{
    losResult res;
    uint32* code;
    size code_size = 0;
    if((res = readFile(path,reinterpret_cast<void**>(&code),&code_size)) != LOS_SUCCESS)
    {
        printf("LIB OS: FileIO Error: %s \"%s\"\n", "vertex shader failed path:",path);
        return res;
    }

    if(!compiled)
        return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code_size;
    createInfo.pCode = code;
    
    if ((*result = vkCreateShaderModule(handle->device, &createInfo, nullptr, module)) != VK_SUCCESS) 
    {
        printf("LIB OS: Vulkan Error: %s \"%s\"\n", "Creating Module:",getError(*result));
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
    }

    return LOS_SUCCESS;
}

losResult refCreateShaderProgram(refHandle handle, refShaderProgram * program, refCreateShaderProgramInfo & info)
{
    VkResult result;
    if (!(*program))
        return LOS_ERROR_HANDLE_IN_USE;
    *program = new refShaderProgram_T();
    
    losResult res;
    VkShaderModule vertex,fragment;
    if((res = createShaderModule(handle,&result,&vertex,info.vertex_shader,info.pre_compiled)) != LOS_SUCCESS)
        return res;

    if((res = createShaderModule(handle,&result,&fragment,info.vertex_shader,info.pre_compiled)) != LOS_SUCCESS)
        return res;

    void* shader_layout;
    size shader_layout_size  = 0;
    if((res = readFile(info.shader_layout,&shader_layout,&shader_layout_size)) != LOS_SUCCESS)
    {
        printf("LIB OS: Vulkan Error: %s\n", "shader layout failed to read");
        return res;
    }

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertex;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragment;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};
    
    return LOS_SUCCESS;
}

losResult refDestroyShaderProgram(refHandle handle, refShaderProgram program)
{
    vkDestroyPipeline(handle->device, program->pipe, nullptr);
    vkDestroyPipelineLayout(handle->device, program->pipe_layout, nullptr);
    return LOS_SUCCESS;
}

