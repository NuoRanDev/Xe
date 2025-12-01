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

		bool init_vulkan_instance(xeString exe_name);

		bool link_physical_device();

		std::vector<Gpu> get_physical_device_list() const noexcept
		{
			return gpu_instance.get_gpu_physical_device_list();
		}

		bool create_logical_device(const xeString& gpu_name, float* pqueue_priorities, int32_t queue_count);

		bool link_window_surface(VkSurfaceKHR surface);

		~VulkanContext();

	private:
		VulkanGpuInstance gpu_instance;

		VkInstance vk_instance;

	};
} // namespace xe is end

#endif // _INC_XE_VK_INSTANCE_HPP_ IS EOF