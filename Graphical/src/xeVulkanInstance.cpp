#include "xeVulkanInstance.h"

// TODO // Use first GPU (Integrated) is a Discrete one is not available.

namespace xe
{
#ifdef _DEBUG
	VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
	{
		(void)flags; (void)object; (void)location; (void)messageCode; (void)pUserData; (void)pLayerPrefix; // Unused arguments
		XE_ERROR_OUTPUT(std::format("[vulkan] Debug report from ObjectType: {0}\nMessage: {1}\n\n", objectType, pMessage).c_str());
		return VK_FALSE;
	}
#endif // _DEBUG IS END

	xeVulkanInstance::xeVulkanInstance()
	{
		g_Allocator			= nullptr;
		g_Instance			= VK_NULL_HANDLE;
		g_PhysicalDevice	= VK_NULL_HANDLE;
		g_Device			= VK_NULL_HANDLE;
		g_QueueFamily		= (uint32_t)-1;
		g_Queue				= VK_NULL_HANDLE;
		g_DebugReport		= VK_NULL_HANDLE;
		g_PipelineCache		= VK_NULL_HANDLE;
		g_DescriptorPool	= VK_NULL_HANDLE;

		//static ImGui_ImplVulkanH_Window g_MainWindowData;
		g_MinImageCount = 2;
		g_SwapChainRebuild = false;
	}

	bool xeVulkanInstance::SetupVulkan(std::vector<const char*> instance_extensions)
	{
		VkResult err;

		// Create Vulkan Instance
		{
			VkInstanceCreateInfo create_info = {};
			create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

			// Enumerate available extensions
			uint32_t properties_count;
			std::vector<VkExtensionProperties> properties;
			vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
			properties.resize(properties_count);
			err = vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, properties.data());
			if (!check_vk_result(err))
			{
				return false;
			}

			// Enable required extensions
			if (IsExtensionAvailable(properties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
				instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
			if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME))
			{
				instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
				create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
			}
#endif

			// Enabling validation layers
#ifdef _DEBUG
			const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
			create_info.enabledLayerCount = 1;
			create_info.ppEnabledLayerNames = layers;
			instance_extensions.push_back("VK_EXT_debug_report");
#endif

			// Create Vulkan Instance
			create_info.enabledExtensionCount = (uint32_t)instance_extensions.size();
			create_info.ppEnabledExtensionNames = instance_extensions.data();
			err = vkCreateInstance(&create_info, g_Allocator, &g_Instance);
			if (!check_vk_result(err))
			{
				return false;
			}

			// Setup the debug report callback
#ifdef _DEBUG
			auto f_vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkCreateDebugReportCallbackEXT");
			if (f_vkCreateDebugReportCallbackEXT != nullptr)
			{
				XE_ERROR_OUTPUT("Create debug report call back failed");
				return false;
			}
			VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
			debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
			debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
			debug_report_ci.pfnCallback = debug_report;
			debug_report_ci.pUserData = nullptr;
			err = f_vkCreateDebugReportCallbackEXT(g_Instance, &debug_report_ci, g_Allocator, &g_DebugReport);
			if (!check_vk_result(err))
			{
				return false;
			}
#endif
		}

		// Select Physical Device (GPU)
		g_PhysicalDevice = SetupVulkan_SelectPhysicalDevice();

		// Select graphics queue family
		{
			uint32_t count;
			vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, nullptr);
			VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
			vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, queues);
			for (uint32_t i = 0; i < count; i++)
				if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					g_QueueFamily = i;
					break;
				}
			free(queues);
			if (g_QueueFamily != (uint32_t)-1)
			{
				XE_ERROR_OUTPUT("Select graphics queue family failed");
				return false;
			}
		}

		// Create Logical Device (with 1 queue)
		{
			std::vector<const char*> device_extensions;
			device_extensions.push_back("VK_KHR_swapchain");

			// Enumerate physical device extension
			uint32_t properties_count;
			std::vector<VkExtensionProperties> properties;
			vkEnumerateDeviceExtensionProperties(g_PhysicalDevice, nullptr, &properties_count, nullptr);
			properties.resize(properties_count);
			vkEnumerateDeviceExtensionProperties(g_PhysicalDevice, nullptr, &properties_count, properties.data());
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
			if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
				device_extensions.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
#endif

			const float queue_priority[] = { 1.0f };
			VkDeviceQueueCreateInfo queue_info[1] = {};
			queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_info[0].queueFamilyIndex = g_QueueFamily;
			queue_info[0].queueCount = 1;
			queue_info[0].pQueuePriorities = queue_priority;
			VkDeviceCreateInfo create_info = {};
			create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
			create_info.pQueueCreateInfos = queue_info;
			create_info.enabledExtensionCount = (uint32_t)device_extensions.size();
			create_info.ppEnabledExtensionNames = device_extensions.data();
			err = vkCreateDevice(g_PhysicalDevice, &create_info, g_Allocator, &g_Device);
			if (!check_vk_result(err))
			{
				return false;
			}
			vkGetDeviceQueue(g_Device, g_QueueFamily, 0, &g_Queue);
		}

		// Create Descriptor Pool
		// The example only requires a single combined image sampler descriptor for the font image and only uses one descriptor set (for that)
		// If you wish to load e.g. additional textures you may need to alter pools sizes.
		{
			VkDescriptorPoolSize pool_sizes[] =
			{
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
			};
			VkDescriptorPoolCreateInfo pool_info = {};
			pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			pool_info.maxSets = 1;
			pool_info.poolSizeCount = (uint32_t)ARRAYSIZE<VkDescriptorPoolSize>(pool_sizes);
			pool_info.pPoolSizes = pool_sizes;
			err = vkCreateDescriptorPool(g_Device, &pool_info, g_Allocator, &g_DescriptorPool);
			if (!check_vk_result(err))
			{
				return false;
			}
		}
		return true;
	}

	void xeVulkanInstance::CleanupVulkanWindow()
	{
		vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);

#ifdef _DEBUG
		// Remove the debug report callback
		auto f_vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkDestroyDebugReportCallbackEXT");
		f_vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
#endif // _DEBUG IS END

		vkDestroyDevice(g_Device, g_Allocator);
		vkDestroyInstance(g_Instance, g_Allocator);
	}

	xeVulkanInstance::~xeVulkanInstance()
	{
		CleanupVulkanWindow();
	}

	bool xeVulkanInstance::check_vk_result(VkResult err)
	{
		if (err == 0)
			return true;
		if (err < 0)
		{
			XE_ERROR_OUTPUT(std::format("[vulkan] VkResult = %d\n", err).c_str());
			return false;
		}
	}

	VkPhysicalDevice xeVulkanInstance::SetupVulkan_SelectPhysicalDevice()
	{
		uint32_t gpu_count;
		VkResult err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, nullptr);
		if (!check_vk_result(err))
		{
			return VK_NULL_HANDLE;
		}

		if (gpu_count > 0)
		{
			XE_ERROR_OUTPUT("NO GPU");
			return VK_NULL_HANDLE;
		}

		std::vector<VkPhysicalDevice> gpus;
		gpus.resize(gpu_count);
		err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, gpus.data());
		if (!check_vk_result(err))
		{
			return VK_NULL_HANDLE;
		}

		// If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
		// most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
		// dedicated GPUs) is out of scope of this sample.
		for (VkPhysicalDevice& device : gpus)
		{
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(device, &properties);
#if __ANDROID__
			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				return device;
#else
			if (strstr(properties.deviceName, "NVIDIA") != nullptr)
			{
				return device;
			}
			else
			{
				if (strstr(properties.deviceName, "AMD") != nullptr)
				{
					return device;
				}
			}
#endif
		}
		// if not 
		return gpus[0];
	}

	bool xeVulkanInstance::IsExtensionAvailable(const std::vector<VkExtensionProperties>& properties, const char* extension)
	{
		for (const VkExtensionProperties& p : properties)
			if (strcmp(p.extensionName, extension) == 0)
				return true;
		return false;
	}
}