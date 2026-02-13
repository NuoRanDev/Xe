#ifndef _INC_XE_VK_SWAPCHAIN_HPP_
#define _INC_XE_VK_SWAPCHAIN_HPP_

#include "vulkan/vulkan.h"

#include "type/xeOrdinals.hpp"
#include "type/xeDataStruction.hpp"

#include <cstdint>

namespace xe
{
	class VulkanSwapChain
	{
	public:
		VulkanSwapChain() 
		{
			vk_swap_chain = nullptr;
		}

		void set_render_area(int32_t h, int32_t w) noexcept;

		bool get_swap_chain_info(VkPhysicalDevice phy_dev, VkSurfaceKHR vk_surface) noexcept;

		bool init_swap_chain(VkDevice vk_dev, VkSurfaceKHR vk_surface, uint32_t image_array_size,
			const uint32_t* pqueue_indexs, uint32_t queue_indexs_count) noexcept;

		bool create_image_view(VkDevice vk_dev);

		void release(VkDevice vk_dev) noexcept;

		VkSwapchainKHR vk_swap_chain;

	private:

		bool get_testure(VkDevice vk_device);

		//
		dynamic_array<VkImage> vk_testure_list;
		dynamic_array<VkImageView> vk_testure_list_view;

		VkSurfaceCapabilitiesKHR surface_capabilities;

		VkExtent2D vk_extent;
		uint32_t vk_image_count;
		VkSurfaceFormatKHR vk_surface_format;
		VkSurfaceTransformFlagBitsKHR vk_surface_transform;
		VkPresentModeKHR vk_present_mod;
	};


} // namespace xe is end

#endif // _INC_XE_VK_SWAPCHAIN_HPP_ IS EOF