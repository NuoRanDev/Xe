#include "graphicsAPI/xeVkSwapChain.hpp"

#include "log/xeLogOutput.hpp"

#include <algorithm>

namespace xe
{
	void VulkanSwapChain::set_render_area(int32_t h, int32_t w) noexcept
	{
		vk_extent.height = std::clamp<uint32_t>(static_cast<uint32_t>(h), surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);
		vk_extent.width = std::clamp<uint32_t>(static_cast<uint32_t>(w), surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
		vk_surface_transform = surface_capabilities.currentTransform;
	}

	bool VulkanSwapChain::get_swap_chain_info(VkPhysicalDevice phy_dev, VkSurfaceKHR vk_surface) noexcept
	{
		uint32_t item_number;
		dynamic_array<VkSurfaceFormatKHR> surface_formats_list;
		dynamic_array<VkPresentModeKHR> vk_present_mod_list;

		vkGetPhysicalDeviceSurfaceFormatsKHR(phy_dev, vk_surface, &item_number, nullptr);
		surface_formats_list.resize(item_number);
		vkGetPhysicalDeviceSurfaceFormatsKHR(phy_dev, vk_surface, &item_number, surface_formats_list.data());

		vkGetPhysicalDeviceSurfacePresentModesKHR(phy_dev, vk_surface, &item_number, nullptr);
		vk_present_mod_list.resize(item_number);
		vkGetPhysicalDeviceSurfacePresentModesKHR(phy_dev, vk_surface, &item_number, vk_present_mod_list.data());

		// SET_SURFACE FORMAT:
		for (const auto& surface_format : surface_formats_list)
		{
			if (surface_format.format == VK_FORMAT_R8G8B8A8_SRGB || surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				vk_surface_format = surface_format;
				goto SET_SURFACE_NUMBER;
			}
		}
		// If supported format not found
		XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI: Vulkan", "Supported format not found!");
		return false;

		SET_SURFACE_NUMBER:
		if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phy_dev, vk_surface, &surface_capabilities)!= VK_SUCCESS)
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI: Vulkan", "Get physical device-surface capabilities failed!");
			return false;
		}
		vk_image_count = std::clamp<uint32_t>(3, surface_capabilities.minImageCount, surface_capabilities.maxImageCount);
		
		vk_present_mod = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
		for (const auto& present_mod : vk_present_mod_list)
		{
			if (present_mod == VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR)
			{
				vk_present_mod = present_mod;
				break;
			}
		}
		return true;
	}

	bool VulkanSwapChain::init_swap_chain(VkDevice vk_dev, VkSurfaceKHR vk_surface, uint32_t image_array_size,
		const uint32_t* pqueue_indexs, uint32_t queue_indexs_count) noexcept
	{
		VkSwapchainCreateInfoKHR create_info{};

		// To be implemented: swap chain creation logic
		create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		create_info.clipped = VK_TRUE;
		create_info.imageArrayLayers = image_array_size;
		create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		create_info.surface = vk_surface;
		create_info.imageColorSpace = vk_surface_format.colorSpace;
		create_info.imageFormat = vk_surface_format.format;
		create_info.imageExtent = vk_extent;
		create_info.minImageCount = vk_image_count;
		create_info.presentMode = vk_present_mod;
		create_info.preTransform = vk_surface_transform;

		create_info.pQueueFamilyIndices = pqueue_indexs;
		create_info.queueFamilyIndexCount = queue_indexs_count;
		create_info.imageSharingMode = VkSharingMode::VK_SHARING_MODE_CONCURRENT;

		if (vkCreateSwapchainKHR(vk_dev, &create_info, nullptr, &vk_swap_chain) != VK_SUCCESS)
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI: Vulkan", "Create swap chain failed!");
			return false;
		}
		return true;
	}

	bool VulkanSwapChain::create_image_view(VkDevice vk_dev)
	{
		for (size_t i = 0; i < vk_testure_list.size(); i++)
		{
			VkImageViewCreateInfo view_info = {};
			view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			view_info.image = (vk_testure_list[i]);
			view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			view_info.format = VK_FORMAT_R8G8B8A8_UNORM;
			view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			view_info.subresourceRange.baseMipLevel = 0;
			view_info.subresourceRange.levelCount = 1;
			view_info.subresourceRange.baseArrayLayer = 0;
			view_info.subresourceRange.layerCount = 1;

			VkImageView texture_image_view;
			if (vkCreateImageView(vk_dev, &view_info, nullptr, &texture_image_view) != VK_SUCCESS)
			{
				XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI: Vulkan", "Failed to create texture image view!");
				continue;
			}
			vk_testure_list_view.push_back(texture_image_view);
		}
		return true;
	}

	void VulkanSwapChain::release(VkDevice vk_dev) noexcept
	{
		for (auto texture_image_view : vk_testure_list_view)
		{
			vkDestroyImageView(vk_dev, texture_image_view, nullptr);
		}

		if (vk_swap_chain != nullptr)
		{
			vkDestroySwapchainKHR(vk_dev, vk_swap_chain, nullptr);
			vk_swap_chain = nullptr;
		}
	}

	bool VulkanSwapChain::get_testure(VkDevice vk_device)
	{
		uint32_t count;
		if (vkGetSwapchainImagesKHR(vk_device, vk_swap_chain, &count, nullptr) != VK_SUCCESS)
			goto IMAGE_NOT_FIND;
		vk_testure_list.resize(count);
		if (vkGetSwapchainImagesKHR(vk_device, vk_swap_chain, &count, vk_testure_list.data()) != VK_SUCCESS)
			goto IMAGE_NOT_FIND;
		return true;
	IMAGE_NOT_FIND:
		XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI: Vulkan", "Can't find vulkan image!");
		return false;
	}
} // namespace xe is end