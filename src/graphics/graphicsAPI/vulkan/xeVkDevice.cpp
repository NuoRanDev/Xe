#include "graphicsAPI/vulkan/xeVkDevice.hpp"

#include "log/xeLogOutput.hpp"

#include <format>

namespace xe
{
	VulkanGpuInstance::VulkanGpuInstance() noexcept
	{
		cur_gpu = nullptr;
		vk_device = nullptr;

		graphics_index = -1;
		present_index = -1;
	}

	bool VulkanGpuInstance::get_gpu_info(VkInstance vk_instance) noexcept
	{
		uint32_t device_count = 0;
		std::vector<VkPhysicalDevice> devices;

		VkPhysicalDeviceProperties device_properties;
		VkPhysicalDeviceFeatures device_features;

		Gpu gpu;
		
		vkEnumeratePhysicalDevices(vk_instance, &device_count, nullptr);
		if (device_count == 0)
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI: Vulkan", "failed to find GPUs with Vulkan support!");
			return false;
		}

		devices.resize(device_count);
		vkEnumeratePhysicalDevices(vk_instance, &device_count, devices.data());

		for (auto& vk_gpu : devices)
		{
			vkGetPhysicalDeviceProperties(vk_gpu, &device_properties);
			vkGetPhysicalDeviceFeatures(vk_gpu, &device_features);

			switch (device_properties.deviceType)
			{
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
				gpu.gpu_type = GPU_TYPE::DISCRETE_GPU;
				break;

			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				gpu.gpu_type = GPU_TYPE::INTEGRATED_GPU;
				break;

			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
				gpu.gpu_type = GPU_TYPE::VIRTUAL_GPU;
				break;

			case VK_PHYSICAL_DEVICE_TYPE_CPU:
				gpu.gpu_type = GPU_TYPE::CPU;
				break;

			default:
				continue;
			}
			gpu.device_name = device_properties.deviceName;
			gpu.vk_physical_device = vk_gpu;
			gpu.is_support_geometry_shader = (device_features.geometryShader == VK_TRUE);
			gpu.is_support_compute_shader = true;

			gpu_list.push_back(gpu);
		}

		if(gpu_list.size() == 0)
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", "failed to find GPUs with Vulkan properties support!");
			return false;
		}
		return true;
	}

	bool VulkanGpuInstance::pick_up_gpu(const String& gpu_name) noexcept
	{
		for (auto& gpu : gpu_list)
		{
			if (gpu.get_device_name() == gpu_name)
			{
				cur_gpu = std::any_cast<VkPhysicalDevice>(gpu.vk_physical_device);
				goto FIND_GPU;
			}
		}
		XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", std::format("failed to find the specified GPU name: {0}", gpu_name.c_str()).c_str());
		return false;

	FIND_GPU:
		get_device_queue_family();
		return true;
	}

	bool VulkanGpuInstance::create_logical_device(VkSurfaceKHR i_vk_surface, float* pqueue_priorities, int32_t queue_count)
	{
		VkDeviceCreateInfo device_create_info = {};

		std::vector<VkDeviceQueueCreateInfo> queue_create_info_list;
		VkDeviceQueueCreateInfo queue_create_info = {};

		VkPhysicalDeviceFeatures device_features = {};
		VkResult result;

		if (!get_device_queue_family_support(i_vk_surface))
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", "failed to find supporting queue!");
			return false;
		}

		if (graphics_index == present_index)
		{
			queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_create_info.pQueuePriorities = pqueue_priorities;
			queue_create_info.queueCount = queue_count;
			queue_create_info.queueFamilyIndex = static_cast<uint32_t>(graphics_index);
			queue_create_info_list.push_back(queue_create_info);
		}
		else
		{
			queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_create_info.pQueuePriorities = pqueue_priorities;
			queue_create_info.queueCount = queue_count;
			queue_create_info.queueFamilyIndex = static_cast<uint32_t>(graphics_index);
			queue_create_info_list.push_back(queue_create_info);
			queue_create_info.queueFamilyIndex = static_cast<uint32_t>(present_index);
			queue_create_info_list.push_back(queue_create_info);
		}

		device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_create_info.pQueueCreateInfos = queue_create_info_list.data();
		device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_info_list.size());
		device_create_info.pEnabledFeatures = &device_features;
		
		result = vkCreateDevice(cur_gpu, &device_create_info, nullptr, &vk_device);
		if (result != VK_SUCCESS)
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", "failed to create logical device!");
			return false;
		}

		return true;
	}

	VulkanQueueArray VulkanGpuInstance::get_graphics_queue() const noexcept
	{
		VulkanQueueArray result;
		VkQueue queue;
		
		vkGetDeviceQueue(vk_device, static_cast<uint32_t>(graphics_index), 0, &queue);
		result.graphics_queue = queue;
		vkGetDeviceQueue(vk_device, static_cast<uint32_t>(present_index), 0, &queue);
		result.present_queue = queue;

		return result;
	}

	void VulkanGpuInstance::release() noexcept
	{
		vkDestroyDevice(vk_device, nullptr);
	}

	void VulkanGpuInstance::get_device_queue_family() noexcept
	{
		uint32_t queue_family_count = 0;

		vkGetPhysicalDeviceQueueFamilyProperties(cur_gpu, &queue_family_count, nullptr);
		queue_family_list = std::vector<VkQueueFamilyProperties>(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(cur_gpu, &queue_family_count, queue_family_list.data());
	}

	bool VulkanGpuInstance::get_device_queue_family_support(VkSurfaceKHR i_vk_surface) noexcept
	{
		VkBool32 present_support = false;

		graphics_index = -1;
		present_index = -1;
		
		for (size_t i = 0; i < queue_family_list.size(); i++)
		{
			const auto& queue_family = queue_family_list[i];
			if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				graphics_index = static_cast<uint32_t>(i);
			vkGetPhysicalDeviceSurfaceSupportKHR(cur_gpu, static_cast<uint32_t>(i), i_vk_surface, &present_support);
			if (present_support)
				present_index = static_cast<uint32_t>(i);
			if (graphics_index != -1 && present_index != -1)
				break;
			i++;
		}
		if (graphics_index == -1)
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", "failed to find a graphics queue family!");
			return false;
		}
		if (present_index==-1)
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", "failed to find a present queue family!");
			return false;
		}
		return true;
	}
} // namespace xe is end