#ifndef _INC_XE_VK_SWAPCHAIN_HPP_
#define _INC_XE_VK_SWAPCHAIN_HPP_

#include "vulkan/vulkan.h"

#include <vector>
#include <cstdint>

namespace xe
{
	class VulkanSwapChain
	{
	public:
		VulkanSwapChain() = default;

		void set_extent(int32_t h, int32_t w) noexcept;

		bool get_swap_chain_info(VkPhysicalDevice phy_dev, VkSurfaceKHR vk_surface) noexcept;

		bool init_swap_chain(int32_t h, int32_t w, VkSurfaceKHR vk_surface, uint32_t image_array_size) noexcept;

		void release() noexcept;

		~VulkanSwapChain() = default;
	private:
		
		VkSwapchainKHR vk_swap_chain;
		VkSurfaceCapabilitiesKHR surface_capabilities;
		VkSurfaceTransformFlagBitsKHR vk_surface_transform;

		VkExtent2D vk_extent;
		uint32_t vk_image_count;
		VkFormat vk_image_format;
	};


} // namespace xe is end

#endif // _INC_XE_VK_SWAPCHAIN_HPP_ IS EOF