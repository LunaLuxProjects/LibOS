#include "../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#define VMA_IMPLEMENTATION 1

#include <RefractileAPI.h>
#include <Components/Defines.h>

#include <cstdio>
#include <vector>

#include "Share.hpp"
losResult createSwapChain(refHandle handle,bool first);
void destroySwapchain(refHandle handle);


void createSwapChainInfo(refHandle& handle,VkSwapchainCreateInfoKHR* swap_chain_create_info,VkSwapchainKHR old_swap_chain) noexcept
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle->physical, handle->surface, &surfaceCapabilities);
    swap_chain_create_info->sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swap_chain_create_info->surface = handle->surface;
    swap_chain_create_info->imageFormat = handle->surface_format.format;
    swap_chain_create_info->imageColorSpace = handle->surface_format.colorSpace;

    uint32 image_count = surfaceCapabilities.minImageCount + 1;
	if (surfaceCapabilities.maxImageCount > 0 && image_count > surfaceCapabilities.maxImageCount) 
        image_count = surfaceCapabilities.maxImageCount;
    swap_chain_create_info->minImageCount = image_count;

    swap_chain_create_info->imageExtent = surfaceCapabilities.maxImageExtent;

    uint32 image_array_layers = 1;
	if (surfaceCapabilities.maxImageArrayLayers < 1)
		image_array_layers = surfaceCapabilities.maxImageArrayLayers;
    swap_chain_create_info->imageArrayLayers = image_array_layers;

    swap_chain_create_info->imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (handle->graphic_family_index !=handle->present_family_index) 
    {
        swap_chain_create_info->imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swap_chain_create_info->queueFamilyIndexCount = 2;
        uint32 queue_family_indices[] = { handle->graphic_family_index, handle->present_family_index };
        swap_chain_create_info->pQueueFamilyIndices = queue_family_indices;
	} 
    else 
		swap_chain_create_info->imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        
    swap_chain_create_info->preTransform = surfaceCapabilities.currentTransform;
    swap_chain_create_info->compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    std::vector<VkPresentModeKHR> available_resent_modes;
    const VkPresentModeKHR desired_present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR,VK_PRESENT_MODE_FIFO_KHR};
    uint32 present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(handle->physical,handle->surface,&present_mode_count,nullptr);
    available_resent_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(handle->physical,handle->surface,&present_mode_count,available_resent_modes.data());
	for (auto const& desired_pm : desired_present_modes) {
		for (auto const& available_pm : available_resent_modes) {
			if (desired_pm == available_pm) present_mode = desired_pm;
		}
	}
    swap_chain_create_info->presentMode = present_mode;
    swap_chain_create_info->clipped = VK_TRUE;
    swap_chain_create_info->oldSwapchain = old_swap_chain;
}

losResult createSwapChain(refHandle handle,bool first)
{
    [[unlikely]]if(handle->closing) return LOS_SUCCESS;
    VkResult result;
    vkQueueWaitIdle(handle->graphics_queue);
    //depth stencil image
    if(handle->used_depth)
    {
        VkImageCreateInfo dimg_info{};
        dimg_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        dimg_info.pNext = nullptr;
        dimg_info.flags = 0;
        dimg_info.imageType = VK_IMAGE_TYPE_2D;
        //FIXME: should be able to change from user
        dimg_info.format = VK_FORMAT_D32_SFLOAT;
        //-----------------------------------------
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle->physical, handle->surface, &surfaceCapabilities);
        dimg_info.extent.width = surfaceCapabilities.currentExtent.width;
        dimg_info.extent.height = surfaceCapabilities.currentExtent.height;
        dimg_info.extent.depth = 1;
        dimg_info.mipLevels = 1;
        dimg_info.arrayLayers = 1;
        dimg_info.samples = static_cast<VkSampleCountFlagBits>(handle->sample_count);
        dimg_info.tiling = VK_IMAGE_TILING_OPTIMAL;
        dimg_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

        // for the depth image, we want to allocate it from GPU local memory
        VmaAllocationCreateInfo dimg_allocinfo = {};
        dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        dimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // allocate and create the image
        vmaCreateImage(handle->vulkan_allocator, &dimg_info, &dimg_allocinfo, &handle->depth_image, &handle->depth_image_alloc, nullptr);

        // build an image-view for the depth image to use for rendering
        VkImageViewCreateInfo dview_info{};
        dview_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        dview_info.pNext = nullptr;
        dview_info.flags = 0;
        dview_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        dview_info.image = handle->depth_image;
        //FIXME: should be able to change from user
        dview_info.format = VK_FORMAT_D32_SFLOAT;
        //-----------------------------------------
        dview_info.subresourceRange.baseMipLevel = 0;
        dview_info.subresourceRange.levelCount = 1;
        dview_info.subresourceRange.baseArrayLayer = 0;
        dview_info.subresourceRange.layerCount = 1;
        dview_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        VK_TEST(vkCreateImageView(handle->device, &dview_info, nullptr, &handle->depth_image_view),LOS_ERROR_COULD_NOT_INIT);
    }
    //swap chain
    {
        VkSwapchainCreateInfoKHR swap_chain_create_info = {};
        VkSwapchainKHR swap_chain = handle->swap_chain;

        {
            VkSurfaceCapabilitiesKHR surfaceCapabilities;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle->physical, handle->surface, &surfaceCapabilities);
            handle->current_screen_size = std::move(surfaceCapabilities.currentExtent);
        }

        if(!first) createSwapChainInfo(handle,&swap_chain_create_info,swap_chain);
        else createSwapChainInfo(handle,&swap_chain_create_info,nullptr);

        VK_TEST(vkCreateSwapchainKHR(handle->device, &swap_chain_create_info, nullptr, &handle->swap_chain),LOS_ERROR_COULD_NOT_INIT);

        if(!first) vkDestroySwapchainKHR(handle->device, swap_chain, nullptr);

        uint32 image_count = 0;
        VK_TEST(vkGetSwapchainImagesKHR(handle->device, handle->swap_chain, &image_count, nullptr),LOS_ERROR_COULD_NOT_INIT);
        handle->swap_chain_images.resize(image_count);
        handle->swap_chain_image_views.resize(image_count);
        VK_TEST(vkGetSwapchainImagesKHR(handle->device, handle->swap_chain, &image_count, handle->swap_chain_images.data()),LOS_ERROR_COULD_NOT_INIT);

        for (uint32_t i = 0; i < image_count; ++i)
        {
            VkImageViewCreateInfo image_view_create_info{};
            image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            image_view_create_info.image = handle->swap_chain_images[i];
            image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            image_view_create_info.format = handle->surface_format.format;
            image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            image_view_create_info.subresourceRange.baseMipLevel = 0;
            image_view_create_info.subresourceRange.levelCount = 1;
            image_view_create_info.subresourceRange.baseArrayLayer = 0;
            image_view_create_info.subresourceRange.layerCount = 1;
            VK_TEST(vkCreateImageView(handle->device, &image_view_create_info, nullptr, &handle->swap_chain_image_views[i]),LOS_ERROR_COULD_NOT_INIT);
            refFrameBuffer frame_buffer;
            losResult _result;
            if((_result = refCreateFrameBuffer(handle,&frame_buffer,&handle->swap_chain_image_views[i])) != LOS_SUCCESS)
                return _result;
            handle->framebuffer.emplace_back(std::move(frame_buffer));
        }
    }
    return LOS_SUCCESS;
}

void destroySwapchain(refHandle handle)
{
    vkDeviceWaitIdle(handle->device);
    if(handle->used_depth)
    {
        vmaDestroyImage(handle->vulkan_allocator,handle->depth_image,handle->depth_image_alloc);
        vkDestroyImageView(handle->device, handle->depth_image_view,nullptr);
    }
    for (auto frame: handle->framebuffer)
        refDestroyFrameBuffer(handle,frame);
    handle->framebuffer.clear();

    for (auto view : handle->swap_chain_image_views)
        vkDestroyImageView(handle->device, view, nullptr);
    handle->swap_chain_image_views.clear();
}

losResult refAppendGraphicsContext(refHandle handle, losWindow window,const refCreateGraphicContextInfo& info) noexcept
{
    {
        VkBool32 support{false};
        VK_TEST(vkGetPhysicalDeviceSurfaceSupportKHR(handle->physical, handle->graphic_family_index, handle->surface, &support),LOS_ERROR_COULD_NOT_INIT)
        [[unlikely]] if (!support)
        {
            printf("LIB OS: Vulkan Error: %s , %s\n", "something is not right with surface creation",getError(result)); 
            return LOS_ERROR_COULD_NOT_INIT;
        }
        uint32 count = 0;
        VK_TEST(vkGetPhysicalDeviceSurfaceFormatsKHR(handle->physical, handle->surface, &count, nullptr),LOS_ERROR_COULD_NOT_INIT)
        [[unlikely]]if (count == 0)
        {
            printf("LIB OS: Vulkan Error: %s\n", "no surface formats found"); 
            return LOS_ERROR_COULD_NOT_INIT;
        }
        std::vector<VkQueueFamilyProperties> queueFamilies;
        uint32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(handle->physical, &queueFamilyCount, nullptr);
        queueFamilies.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(handle->physical, &queueFamilyCount, queueFamilies.data());
        std::vector<VkSurfaceFormatKHR> formats(count);
        VK_TEST(vkGetPhysicalDeviceSurfaceFormatsKHR(handle->physical, handle->surface, &count, formats.data()),LOS_ERROR_COULD_NOT_INIT)


        for (const auto& available_format : formats)
        {
            if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                VkFormatProperties properties;
				vkGetPhysicalDeviceFormatProperties(handle->physical, VK_FORMAT_B8G8R8A8_SRGB, &properties);
				if ((properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) == VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)
                   handle->surface_format = available_format;
            }
        }

        if(handle->surface_format.format == VK_FORMAT_UNDEFINED)
        {
            printf("LIB OS: Vulkan Error: %s\n", "no surface formats found"); 
            return LOS_ERROR_COULD_NOT_INIT;
        }      
    }
    //render pass
    {
        VkRenderPassCreateInfo render_pass_info{};
        VkSubpassDescription subpass{};
        VkAttachmentDescription colour_attachment{};
        VkAttachmentReference colour_attachment_ref{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_info.pNext = nullptr;
        render_pass_info.flags = 0;
        colour_attachment.format = handle->surface_format.format;
        colour_attachment.samples = static_cast<VkSampleCountFlagBits>(info.sample_count);
        colour_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colour_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colour_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colour_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colour_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colour_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        colour_attachment_ref.attachment = 0;
        colour_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        if(info.has_depth_stencil)
        {
            VkAttachmentReference depth_attachment_ref{};
            depth_attachment_ref.attachment = 1;
            depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentDescription depth_attachment{};
            //FIXME: should be able to change from user
            depth_attachment.format = VK_FORMAT_D32_SFLOAT;
            //-------------------------------
            depth_attachment.samples = static_cast<VkSampleCountFlagBits>(info.sample_count);
            depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            VkAttachmentDescription attachments[2] = {colour_attachment, depth_attachment};
            render_pass_info.attachmentCount = 2;
            render_pass_info.pAttachments = attachments;
            subpass.pDepthStencilAttachment = &depth_attachment_ref;
        }
        else
        {
            render_pass_info.attachmentCount = 1;
            render_pass_info.pAttachments = &colour_attachment;
        }

        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colour_attachment_ref;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        if(info.has_depth_stencil)
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        else
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass;
        render_pass_info.dependencyCount = 1;
        render_pass_info.pDependencies = &dependency;

        VK_TEST(vkCreateRenderPass(handle->device, &render_pass_info, nullptr, &handle->pass),LOS_ERROR_COULD_NOT_INIT);
    }
    losResult _result;
    if((_result = createSwapChain(handle,true)) != LOS_SUCCESS)
        return _result;

    window->window->resize_callback = [](refHandle handle,uint64, uint64) 
    {
        [[unlikely]]if(handle->closing) return LOS_SUCCESS;
        destroySwapchain(handle);
        return createSwapChain(handle,false);
    };
        VK_TEST(vkCreateSemaphore(handle->device, &semaphore_create_info, nullptr, &handle->present_semaphore),LOS_ERROR_COULD_NOT_INIT);
        VK_TEST(vkCreateSemaphore(handle->device, &semaphore_create_info, nullptr, &handle->render_semaphore),LOS_ERROR_COULD_NOT_INIT);
}

losResult refUnAppendGraphicsContext(refHandle handle) noexcept
{
    destroySwapchain(handle);
    vkDestroyRenderPass(handle->device, handle->pass,nullptr);
    vmaDestroyAllocator(handle->vulkan_allocator);
}
#endif