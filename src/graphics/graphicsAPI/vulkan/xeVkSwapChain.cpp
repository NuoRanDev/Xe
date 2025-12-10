#include "graphicsAPI/vulkan/xeVkSwapChain.hpp"

#include "log/xeLogOutput.hpp"

#include <algorithm>

namespace xe
{
	void VulkanSwapChain::set_extent(int32_t h, int32_t w) noexcept
	{
		vk_extent.height = std::clamp<uint32_t>(static_cast<uint32_t>(h), surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);
		vk_extent.width = std::clamp<uint32_t>(static_cast<uint32_t>(w), surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
		vk_surface_transform = surface_capabilities.currentTransform;
	}

	bool VulkanSwapChain::get_swap_chain_info(VkPhysicalDevice phy_dev, VkSurfaceKHR vk_surface) noexcept
	{
		uint32_t surface_number;
		std::vector<VkSurfaceFormatKHR> surface_formats_list;

		vkGetPhysicalDeviceSurfaceFormatsKHR(phy_dev, vk_surface, &surface_number, nullptr);
		surface_formats_list.resize(surface_number);
		vkGetPhysicalDeviceSurfaceFormatsKHR(phy_dev, vk_surface, &surface_number, surface_formats_list.data());

		// SET_SURFACE FORMAT:
		for (auto& surface_format : surface_formats_list)
		{
			if (surface_format.format == VK_FORMAT_R8G8B8A8_SRGB && surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				vk_image_format = surface_format.format;
				goto SET_SURFACE_NUMBER;
			}
		}
		// If supported format not found
		XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI: Vulkan", "supported format not found!");
		return false;

		SET_SURFACE_NUMBER:
		if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phy_dev, vk_surface, &surface_capabilities)!= VK_SUCCESS)
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI: Vulkan", "Get physical device-surface capabilities failed!");
			return false;
		}
		vk_image_count = std::clamp<uint32_t>(2, surface_capabilities.minImageCount, surface_capabilities.maxImageCount);

		return true;
	}

	bool VulkanSwapChain::init_swap_chain(int32_t h, int32_t w, VkSurfaceKHR vk_surface, uint32_t image_array_size) noexcept
	{
		VkSwapchainCreateInfoKHR create_info{};

		// To be implemented: swap chain creation logic
		create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		create_info.clipped = VK_TRUE;
		create_info.imageArrayLayers = image_array_size;
		create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		create_info.surface = vk_surface;


		return false;
	}
	void VulkanSwapChain::release() noexcept
	{}
} // namespace xe is end