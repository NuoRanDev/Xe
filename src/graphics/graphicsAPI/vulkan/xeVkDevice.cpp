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
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGeometry: Vulkan", "failed to find GPUs with Vulkan support!");
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
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGeometry : VULKAN", "failed to find GPUs with Vulkan properties support!");
			return false;
		}
		return true;
	}

	bool VulkanGpuInstance::pick_up_gpu(const xeString& gpu_name) noexcept
	{
		for (auto& gpu : gpu_list)
		{
			if (gpu.get_device_name() == gpu_name)
			{
				cur_gpu = std::any_cast<VkPhysicalDevice>(gpu.vk_physical_device);
				goto FIND_GPU;
			}
		}
		XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGeometry : VULKAN", std::format("failed to find the specified GPU name: {0}", gpu_name.c_str()).c_str());
		return false;

	FIND_GPU:
		vk_queue.get_device_queue(cur_gpu);
		return true;
	}

	bool VulkanGpuInstance::create_graphics_queue(float* pqueue_priorities, int32_t queue_count)
	{
		VkDeviceCreateInfo device_create_info = {};
		VkDeviceQueueCreateInfo queue_create_info = {};

		VkPhysicalDeviceFeatures device_features = {};
		VkResult result;

		graphics_index = vk_queue.get_support_queue_family(VK_QUEUE_GRAPHICS_BIT);

		queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.pQueuePriorities = pqueue_priorities;
		queue_create_info.queueCount = queue_count;
		queue_create_info.queueFamilyIndex = static_cast<uint32_t>(graphics_index);

		device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_create_info.pQueueCreateInfos = &queue_create_info;
		device_create_info.queueCreateInfoCount = 1;
		device_create_info.pEnabledFeatures = &device_features;
		
		result = vkCreateDevice(cur_gpu, &device_create_info, nullptr, &vk_device);
		if (result != VK_SUCCESS)
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGeometry : VULKAN", "failed to create logical device!");
			return false;
		}

		return true;
	}

	VkQueue VulkanGpuInstance::get_graphics_queue() const noexcept
	{
		VkQueue graphics_queue;
		vkGetDeviceQueue(vk_device, static_cast<uint32_t>(graphics_index), 0, &graphics_queue);
		return graphics_queue;
	}

	void VulkanGpuInstance::release()
	{
		vkDestroyDevice(vk_device, nullptr);
	}
} // namespace xe is end