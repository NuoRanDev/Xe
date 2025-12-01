#include "graphicsAPI/vulkan/xeVkQueue.hpp"

#include "log/xeLogOutput.hpp"

namespace xe
{
	void VulkanQueue::get_device_queue(VkPhysicalDevice& physical_device)
	{
		uint32_t queue_family_count = 0;

		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
		queue_family_list = std::vector<VkQueueFamilyProperties>(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_family_list.data());
	}

	int64_t VulkanQueue::get_support_queue_family(VkQueueFlagBits index)
	{
		int64_t graphics_index = -1;
		for (int64_t i = 0; i < queue_family_list.size(); i++)
		{
			const auto& queue_family = queue_family_list[i];
			if (queue_family.queueFlags & index)
			{
				graphics_index = i;
				break;
			}
			i++;
		}
		if (graphics_index == -1)
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGeometry : VULKAN", "failed to find a graphics queue family!");
		}
		return graphics_index;
	}
} // namespace xe is end