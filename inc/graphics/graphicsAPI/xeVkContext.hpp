#ifndef _INC_XE_VK_CONTEXT_HPP_
#define _INC_XE_VK_CONTEXT_HPP_

#include "vulkan/vulkan.h"

#include "string/xeString.hpp"
#include "type/xeDataStruction.hpp"

#include "graphicsAPI/xeVkConfig.hpp"
#include "graphicsAPI/xeVkSwapChain.hpp"
#include "SDL3/SDL_vulkan.h"

#include <any>
#include <functional>

namespace xe
{
	namespace vulkan
	{
		class VulkanContext
		{
		public:
			VulkanContext() 
			{
				instance = nullptr;
				vk_device = nullptr;
				window_surface = nullptr;
			}

			bool init_vulkan_instance(const char* const* extension, uint32_t extension_number, const utf8_t* window_name) noexcept;

			bool find_physical_device(const dynamic_array<const char*>& input_want_extension_properties) noexcept;

			bool pick_up_physical_device(const String& gpu_name) noexcept;

			bool create_logical_device(float* pqueue_priorities, int32_t queue_count) noexcept;

			bool get_cur_window_surface(
				std::function<bool(SDL_Window*, VkInstance, VkAllocationCallbacks*, VkSurfaceKHR*)> get_sdl_window_surface, 
				SDL_Window* sdl_window) noexcept;

			bool create_swap_chian(int32_t h, int32_t w);

			void release_surface() noexcept;

			~VulkanContext();

		private:

			bool get_device_queue_family() noexcept;

			bool get_device_queue_family_support() noexcept;

			// vulkan base
			VkInstance instance;
			VkDevice vk_device;
			// use NO.0 gpu to default gpu
			uint64_t device_index = DEFAULT_DEVICE_INDEX;
			dynamic_array<std::pair<String, dynamic_array<char*>>> gpu_info_list;
			dynamic_array<VkPhysicalDevice> phy_dev_tree;
			dynamic_array<const char*> want_extension_properties;

			// any of the queues can be null if this context doesn't intend to use them
			VkQueue graphics_queue = nullptr;
			uint64_t graphics_queue_index = -1;
			VkQueue transfer_queue = nullptr;
			uint64_t transfer_queue_index = -1;
			VkQueue compute_queue = nullptr;
			uint64_t compute_queue_index = -1;
			VkQueue present_queue = nullptr;
			uint64_t present_queue_index = -1;
			dynamic_array<uint32_t> queue_index_list;

			// device queue
			dynamic_array<VkQueueFamilyProperties> queue_family_list;

			// window surface
			VkSurfaceKHR window_surface;

			VulkanSwapChain vk_swap_chain_context;

		};
	} // namespace vulkan is end
} // namespace xe is end

#endif // _INC_XE_VK_CONTEXT_HPP_ IS EOF