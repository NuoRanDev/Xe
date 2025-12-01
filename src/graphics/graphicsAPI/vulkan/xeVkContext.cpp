#include "graphicsAPI/vulkan/xeVkContext.hpp"

#include "graphicsAPI/vulkan/xeVkConfig.hpp"

#include "log/xeLogOutput.hpp"

#include <format>

namespace xe
{
	VulkanContext::VulkanContext()
	{
		vk_instance = nullptr;
	}
	bool VulkanContext::init_vulkan_instance(xeString exe_name)
	{
		VkResult result;
		VkApplicationInfo application_info = {};
		VkInstanceCreateInfo instance_create_info = {};

		uint32_t cur_vulkan_version = vulkan_version;

		if (vkEnumerateInstanceVersion != nullptr && vkEnumerateInstanceVersion(&cur_vulkan_version) != VkResult::VK_SUCCESS)
		{
			cur_vulkan_version = VK_MAKE_API_VERSION(0, 1, 0, 0);
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGeometry: Vulkan", "Vulkan support is low!")
		}

		vk_instance = nullptr;

		application_info.sType = VkStructureType::VK_STRUCTURE_TYPE_APPLICATION_INFO;
		application_info.pNext = nullptr;
		application_info.pApplicationName = exe_name.c_str();
		application_info.applicationVersion = 0;
		application_info.pEngineName = "xe";
		application_info.engineVersion = 0;
		application_info.apiVersion = cur_vulkan_version;

		std::vector<const char*> enable_extension_names;
		enable_extension_names.push_back("VK_KHR_surface");
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64) // Windows
		enable_extension_names.push_back("VK_KHR_win32_surface");
#elif
#endif
	
		instance_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_create_info.pNext = nullptr;
		instance_create_info.flags = 0;
		instance_create_info.pApplicationInfo = &application_info;
		instance_create_info.enabledExtensionCount = static_cast<uint32_t>(enable_extension_names.size());
		instance_create_info.ppEnabledExtensionNames = enable_extension_names.data();

#if defined(ENABLE_VULKAN_VALIDATION)
		if(is_support_enable_validation())
		{
			instance_create_info.enabledLayerCount = 1;
			instance_create_info.ppEnabledLayerNames = &(VULKAN_VALIDATION);
		}
#endif // ENABLE_VULKAN_VALIDATION IS END

		result = vkCreateInstance(&instance_create_info, nullptr, &vk_instance);
		if (result != VK_SUCCESS)
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGeometry: Vulkan", "Vulkan instance init failed!");
			return false;
		}
		return true;
	}

	bool VulkanContext::link_physical_device()
	{
		if (!(gpu_instance.get_gpu_info(vk_instance)))
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGeometry: Vulkan", "GPU instance init failed!");
			return false;
		}
		return true;
	}

	bool VulkanContext::create_logical_device(const xeString& gpu_name, float* pqueue_priorities, int32_t queue_count)
	{
		if (!(gpu_instance.pick_up_gpu(gpu_name, pqueue_priorities, queue_count)))
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGeometry: Vulkan", "Logical device create failed!");
			return false;
		}
#if defined(_DEBUG)
		XE_INFO_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGeometry: Vulkan", std::format("Selected GPU: {0}", gpu_name.c_str()).c_str());
#endif // defined(_DEBUG)  IS END
		return true;
	}

	bool VulkanContext::link_window_surface(VkSurfaceKHR surface)
	{
		return false;
	}

	VulkanContext::~VulkanContext()
	{
		if (vk_instance != nullptr)
		{
			vkDestroyInstance(vk_instance, nullptr);
			vk_instance = nullptr;
		}
	}
} // namespace xe is end