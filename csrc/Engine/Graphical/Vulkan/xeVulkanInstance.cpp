import xe.Graphical.Vulkan;

import std;

import xe.Core.CoreClrOutput;
import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;
import xe.Core.xeString;

import "vulkan/vulkan_core.h";

namespace xe
{
	xeVulkanContext::xeVulkanContext()
	{
		vulkan_instance = VK_NULL_HANDLE;

		physical_device_index = 0;
		physical_device = VK_NULL_HANDLE;
		graphics_queue_family_index = -1;
		present_queue_family_index = -1;


		virtual_logical_device = VK_NULL_HANDLE;
		window_surface = VK_NULL_HANDLE;
	}

	bool xeVulkanContext::SetVulkanInstanceContext(const char* const* instance_extensions, xeUint32 instance_extensions_count, xeString context_name)
	{
#if defined(_DEBUG)
		CheckValidationLayerSupport();
#endif // _DEBUG
		if (!CreateInstance(instance_extensions, instance_extensions_count, xeString(context_name + "vulkan instance").c_str()))
		{
			XE_ERROR_OUTPUT("<LIB: VULKAN> Failed to create instance");
			return false;
		}
		return true;
	}

	bool xeVulkanContext::SetVulkanLogicalDevice()
	{
		if (!SetupVulkanSelectPhysicalDevice())
		{
			XE_ERROR_OUTPUT("<LIB: VULKAN> Failed to select physical device");
			return false;
		}
		if (!CheckQueueFamily())
		{
			XE_ERROR_OUTPUT("<LIB: VULKAN> Failed to set queue family");
			return false;
		}
		if (!CreateLogicalDevice())
		{
			XE_ERROR_OUTPUT("<LIB: VULKAN> Failed to create logical device");
			return false;
		}
		return true;
	}

	void xeVulkanContext::CleanupVulkanContext()
	{
		vkDestroyDevice(virtual_logical_device, nullptr);
#if defined(_DEBUG)
		if (enable_validation_layers)
		{
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vulkan_instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr) 
			{
				func(vulkan_instance, debug_messenger, nullptr);
			}
		}
#endif // _DEBUG
		vkDestroySurfaceKHR(vulkan_instance, window_surface, nullptr);
		
		vkDestroyInstance(vulkan_instance, nullptr);
	}

	xeVulkanContext::~xeVulkanContext()
	{
		CleanupVulkanContext();
	}

#if defined(_DEBUG)
	VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, xeUint64 object, size_t location, xeInt32 messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
	{
		(void)flags; (void)object; (void)location; (void)messageCode; (void)pUserData; (void)pLayerPrefix; // Unused arguments
		XE_ERROR_OUTPUT(std::format("[vulkan] Debug report from ObjectType: {0} Message: {1}", static_cast<xeInt64>(objectType), pMessage).c_str());
		return VK_FALSE;
	}

	bool xeVulkanContext::CheckValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layer_name : validation_layers)
		{
			for (const auto& layerProperties : availableLayers)
			{
				if (std::strcmp(layer_name, layerProperties.layerName) == 0)
				{
					enable_validation_layers = true;
					return true;
				}
			}
		}
		XE_WARNING_OUTPUT("<LIB: vulkan> validation layer not support");
		return false;
	}
#endif // _DEBUG IS END

	bool xeVulkanContext::CheckVulkanError(VkResult err)
	{
		if (err == 0)
			return true;
		if (err < 0)
		{
			XE_ERROR_OUTPUT(std::format("<LIB: vulkan> VkResult = {}", static_cast<xeInt64>(err)).c_str());
			return false;
		}
		return true;
	}

	bool xeVulkanContext::CreateInstance(const char* const* instance_extensions, xeUint32 instance_extensions_count, const char* instance_name)
	{
		// main application info
		VkApplicationInfo app_info = {};
		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pApplicationName = instance_name;
		app_info.pEngineName = "XE";
		//
		VkInstanceCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.pApplicationInfo = &app_info;
		// load gui instance extensions
		create_info.enabledExtensionCount = instance_extensions_count;
		create_info.ppEnabledExtensionNames = instance_extensions;
		create_info.enabledLayerCount = 0;
		create_info.ppEnabledLayerNames = nullptr;
#if defined(_DEBUG)
		if (enable_validation_layers)
		{
			create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
			create_info.ppEnabledLayerNames = validation_layers.data();
		}
#endif // DEBUG
		uint32_t extension_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
		std::vector<VkExtensionProperties> extensions(extension_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
		for (xeSize i = 0; i < extension_count; i++)
		{
			XE_INFO_OUTPUT(std::format("support available extension [{0}] : {1}", i, extensions[1].extensionName).c_str());
		}

		VkResult state = vkCreateInstance(&create_info, nullptr, &vulkan_instance);
		if (!CheckVulkanError(state)) return false;

		return true;
	}

	bool xeVulkanContext::SetupVulkanSelectPhysicalDevice()
	{
		xeUint32 gpu_count;
		VkResult state = vkEnumeratePhysicalDevices(vulkan_instance, &gpu_count, nullptr);
		if (!CheckVulkanError(state)) return false;
		if (gpu_count == 0)
		{
			XE_ERROR_OUTPUT("<LIB: VULKAN> NO GPU");
			return false;
		}
		std::vector<VkPhysicalDevice> gpus;
		gpus.resize(gpu_count);
		state = vkEnumeratePhysicalDevices(vulkan_instance, &gpu_count, gpus.data());
		if (!CheckVulkanError(state)) return false;

		// If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
		// most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
		// dedicated GPUs) is out of scope of this sample.
		// ingore the intel gpu
		std::vector<const char*>corporation_names = 
		{ "NVIDIA","AMD" };
		
#if __ANDROID__
		for (VkPhysicalDevice& device : gpus)
		{
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(device, &properties);
			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				return device;
		}
#else
		XE_INFO_OUTPUT(std::format("Found {0} GPU", gpus.size()).c_str());
		bool is_found = false;
		for (auto& corporation_name: corporation_names)
		{
			for (xeUint32 i = 0; i < gpus.size(); i++)
			{
				VkPhysicalDeviceProperties properties;
				vkGetPhysicalDeviceProperties(gpus[i], &properties);
				XE_INFO_OUTPUT(std::format("Found \t GPU[{0}] {1}", i, properties.deviceName).c_str());
				if (is_found || std::strstr(properties.deviceName, corporation_name) != nullptr)
				{
					XE_INFO_OUTPUT(std::format("Using \t GPU[{0}] {1}", i, properties.deviceName).c_str());
					physical_device = gpus[i];
					physical_device_index = i;
					is_found = true;
				}
			}
			if (is_found) return true;
		}
#endif
		// if not 
		physical_device = gpus[0];
		physical_device_index = 0;
		return true;
	}

	bool xeVulkanContext::CheckQueueFamily()
	{
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

		std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

		for (xeInt32 i = 0; i < queue_families.size(); i++)
		{
			if (queue_families[i].queueCount > 0 && queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) 
			{
				graphics_queue_family_index = i;
			}
			VkBool32 present_support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, window_surface, &present_support);
			if (present_support) present_queue_family_index = i;

			if (present_queue_family_index != -1 && graphics_queue_family_index != -1)
				return true;
		}
		XE_ERROR_OUTPUT("<LIB: VULKAN> Failed to find a suitable GPU");
		return false;
	}

	bool xeVulkanContext::CreateLogicalDevice()
	{

		std::vector<VkDeviceQueueCreateInfo> queue_create_infos_list;
		std::set<xeInt32> unique_queue_families = { graphics_queue_family_index, present_queue_family_index };

		//-------------------------------------------------------------------------------------------------优化
		float queue_qriority = 1.0f;
		for (uint32_t queue_family : unique_queue_families) 
		{
			VkDeviceQueueCreateInfo queue_create_info{};
			queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_create_info.queueFamilyIndex = queue_family;
			queue_create_info.queueCount = 1;
			queue_create_info.pQueuePriorities = &queue_qriority;
			queue_create_infos_list.push_back(queue_create_info);
		}

		VkPhysicalDeviceFeatures device_features{};

		VkDeviceCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos_list.size());
		create_info.pQueueCreateInfos = queue_create_infos_list.data();
		create_info.pEnabledFeatures = &device_features;
		create_info.enabledExtensionCount = 0;
		create_info.enabledLayerCount = 0;
#if defined(_DEBUG)
		if (enable_validation_layers)
		{
			create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
			create_info.ppEnabledLayerNames = validation_layers.data();
		}
#endif // _DEBUG

		if (vkCreateDevice(physical_device, &create_info, nullptr, &virtual_logical_device) != VK_SUCCESS)
		{
			XE_ERROR_OUTPUT("<LIB: VULKAN> Failed to create logical device!");
			return false;
		}

		vkGetDeviceQueue(virtual_logical_device, graphics_queue_family_index, 0, &graphics_queue);
		vkGetDeviceQueue(virtual_logical_device, present_queue_family_index, 0, &present_queue);
		return true;
	}

}// namespace xe IS END