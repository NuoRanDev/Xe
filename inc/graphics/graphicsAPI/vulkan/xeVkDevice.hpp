#ifndef _INC_XE_VK_DEVICE_HPP_
#define _INC_XE_VK_DEVICE_HPP_

#include "type/xeOrdinals.hpp"
#include "string/xeString.hpp"

#include <vector>
#include <unordered_map>
#include <any>

#include "vulkan/vulkan.h"
#include <optional>

#include "graphicsAPI/xePhysicalDevice.hpp"

namespace xe
{
	struct VulkanQueueArray
	{
		VkQueue present_queue;
		VkQueue graphics_queue;
	};

	class VulkanGpuInstance
	{
	public:
		VulkanGpuInstance() noexcept;

		bool get_gpu_info(VkInstance vk_instance) noexcept;

		const std::vector<Gpu> get_gpu_physical_device_list() const noexcept { return gpu_list; }

		bool pick_up_gpu(const xeString& gpu_name) noexcept;
		//const std::vector<const char*>& extension, 
		bool create_logical_device(VkSurfaceKHR i_vk_surface, float* pqueue_priorities, int32_t queue_count);

		VulkanQueueArray get_graphics_queue() const noexcept;

		void release() noexcept;

		~VulkanGpuInstance() = default;

	private:

		void get_device_queue_family() noexcept;

		bool get_device_queue_family_support(VkSurfaceKHR i_vk_surface) noexcept;

		std::vector<VkQueueFamilyProperties> queue_family_list;

		std::vector<Gpu> gpu_list;

		VkPhysicalDevice cur_gpu;

		VkDevice vk_device;

		int64_t graphics_index;
		int64_t present_index;
	};
} // namespace xe is end

#endif // _INC_XE_VK_DEVICE_HPP_ IS EOF