import xe.Graphical.Vulkan;

import std;

import xe.Core.CoreClrOutput;
import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;
import xe.Core.xeString;

import "vulkan/vulkan_core.h";


namespace xe
{
#ifdef _DEBUG
	VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, xeUint64 object, size_t location, xeInt32 messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
	{
		(void)flags; (void)object; (void)location; (void)messageCode; (void)pUserData; (void)pLayerPrefix; // Unused arguments
		XE_ERROR_OUTPUT(std::format("[vulkan] Debug report from ObjectType: {0} Message: {1}", static_cast<xeInt64>(objectType), pMessage).c_str());
		return VK_FALSE;
	}

	bool CheckValidationLayerSupport(VkInstanceCreateInfo& create_info)
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

		for (const char* layerName : validationLayers) {
			for (const auto& layerProperties : availableLayers) {
				if (std::strcmp(layerName, layerProperties.layerName) == 0) {
					goto ADD_VALIDATION_LAYERS_IN_CREATE_INFO;
				}
			}
		}
		XE_WARNING_OUTPUT("validation layer not support");
		return false;
	ADD_VALIDATION_LAYERS_IN_CREATE_INFO:
		create_info.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		create_info.ppEnabledLayerNames = validationLayers.data();
		return true;
	}
#endif // _DEBUG IS END

	xeVulkanContext::xeVulkanContext()
	{
		vulkan_instance = VK_NULL_HANDLE;
		physical_device = VK_NULL_HANDLE;;
	}

	bool xeVulkanContext::SetupVulkanContext(const char* const* instance_extensions, xeUint32 instance_extensions_count, xeString context_name)
	{
		if (!CreateInstance(instance_extensions, instance_extensions_count, xeString(context_name + "vulkan instance").c_str()))
		{
			XE_ERROR_OUTPUT("<LIB VULKAN> Failed to create instance");
			return false;
		}
		if (!SetupVulkanSelectPhysicalDevice())
		{
			XE_ERROR_OUTPUT("<LIB VULKAN> Failed to select physical device");
			return false;
		}
		return true;
	}

	void xeVulkanContext::CleanupVulkanContext()
	{
		vkDestroyInstance(vulkan_instance, nullptr);
	}

	xeVulkanContext::~xeVulkanContext()
	{
		CleanupVulkanContext();
	}

	bool xeVulkanContext::CheckVulkanError(VkResult err)
	{
		if (err == 0)
			return true;
		if (err < 0)
		{
			XE_ERROR_OUTPUT(std::format("LIB<vulkan> VkResult = {}", static_cast<xeInt64>(err)).c_str());
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
#if defined(_DEBUG)
		if (CheckValidationLayerSupport(create_info))
		{
			goto CREATE_INSTANCE;
		}
#endif // DEBUG
		create_info.enabledLayerCount = 0;
		create_info.ppEnabledLayerNames = nullptr;
#if defined(_DEBUG)
	CREATE_INSTANCE:
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
			XE_ERROR_OUTPUT("<LIB VULKAN> NO GPU");
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
		for (auto& corporation_name: corporation_names)
		{
			for (xeSize i = 0; i < gpus.size(); i++)
			{
				VkPhysicalDeviceProperties properties;
				vkGetPhysicalDeviceProperties(gpus[i], &properties);
				XE_INFO_OUTPUT(std::format("Found \t GPU[{0}] {1}", i, properties.deviceName).c_str());
				if (std::strstr(properties.deviceName, corporation_name) != nullptr)
				{
					XE_INFO_OUTPUT(std::format("Using \t GPU[{0}] {1}", i, properties.deviceName).c_str());
					physical_device = gpus[i];
					physical_device_index = i;
					return true;
				}
			}
		}
#endif
		// if not 
		physical_device = gpus[0];
		physical_device_index = 0;
		return true;
	}

	bool xeVulkanContext::SetQueueFamily()
	{
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

		std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

		for (xeSize i = 0; i < queue_families.size(); i++)
		{
			if (queue_families[i].queueCount > 0 && queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) 
			{
				return true;
			}
		}

		return false;
	}

}// namespace xe IS END