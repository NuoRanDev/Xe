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

	void VulkanSwapChain::release(VkDevice vk_dev) noexcept
	{
		if (vk_swap_chain != nullptr)
		{
			vkDestroySwapchainKHR(vk_dev, vk_swap_chain, nullptr);
			vk_swap_chain = nullptr;
		}
	}
} // namespace xe is end