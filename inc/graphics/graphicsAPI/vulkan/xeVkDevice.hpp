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
	class VulkanGpuInstance
	{
	public:
		VulkanGpuInstance() noexcept;

		bool get_gpu_info(VkInstance vk_instance) noexcept;

		const std::vector<Gpu> const get_gpu_physical_device_list() const noexcept { return gpu_list; }

		bool pick_up_gpu(const xeString& gpu_name, float* pqueue_priorities, int32_t queue_count) noexcept;

		VkQueue get_graphics_queue() const noexcept;

		~VulkanGpuInstance();

	private:

		std::vector<Gpu> gpu_list;

		VkPhysicalDevice cur_gpu;

		VkDevice vk_device;

		int64_t graphics_index;
		int64_t present_index;
	};
} // namespace xe is end

#endif // _INC_XE_VK_DEVICE_HPP_ IS EOF