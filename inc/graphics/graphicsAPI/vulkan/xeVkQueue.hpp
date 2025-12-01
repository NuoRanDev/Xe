#ifndef _INC_XE_VK_QUEUE_HPP_
#define _INC_XE_VK_QUEUE_HPP_

#include "vulkan/vulkan.h"
#include "SDL3/SDL_vulkan.h"

#include <vector>
#include <cstdint>

namespace xe
{
	class VulkanQueue
	{
	public:
		VulkanQueue() = default;

		void get_device_queue(VkPhysicalDevice& physical_device);

		int64_t get_support_queue_family(VkQueueFlagBits index);

		~VulkanQueue() = default;

	private:
		
		std::vector<VkQueueFamilyProperties> queue_family_list;
	};


} // namespace xe is end

#endif // _INC_XE_VK_QUEUE_HPP_ IS EOF