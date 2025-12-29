#ifndef _INC_XE_VK_INSTANCE_HPP_
#define _INC_XE_VK_CONTEXT_HPP_

#include "vulkan/vulkan.h"

#include "string/xeString.hpp"

#include "graphicsAPI/vulkan/xeVkConfig.hpp"
#include "graphicsAPI/vulkan/xeVkDevice.hpp"

#include <vector>

namespace xe
{
	class VulkanContext
	{
	public:
		VulkanContext();

		bool init_vulkan_instance(const char* const* extension, uint32_t extension_number, String exe_name);

		bool link_physical_device();

		std::vector<Gpu> get_physical_device_list() const noexcept
		{
			return gpu_instance.get_gpu_physical_device_list();
		}

		bool pick_up_physical_device(const String& gpu_name) noexcept;

		bool create_logical_device(float* pqueue_priorities, int32_t queue_count);

		void link_window_surface(VkSurfaceKHR i_vk_surface)
		{
			vk_surface = i_vk_surface;
		}

		void release_surface();
		
		~VulkanContext();

	private:

		friend class Window;

		VulkanGpuInstance gpu_instance;

		VkInstance vk_instance;

		VkSurfaceKHR vk_surface;

	};
} // namespace xe is end

#endif // _INC_XE_VK_INSTANCE_HPP_ IS EOF