#include "graphicsAPI/xeVkContext.hpp"

#include "graphicsAPI/xeVkConfig.hpp"

#include "log/xeLogOutput.hpp"

#include <format>

namespace xe
{
	namespace vulkan
	{
		bool VulkanContext::init_vulkan_instance(const char* const* extension, uint32_t extension_number, const utf8_t* window_name) noexcept
		{
			VkResult result;
			VkApplicationInfo application_info = {};
			VkInstanceCreateInfo instance_create_info = {};

			uint32_t cur_vulkan_version = vulkan_version;

			if (vkEnumerateInstanceVersion != nullptr && vkEnumerateInstanceVersion(&cur_vulkan_version) != VkResult::VK_SUCCESS)
			{
				cur_vulkan_version = VK_MAKE_API_VERSION(0, 1, 0, 0);
				XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI: Vulkan", "Vulkan support is low!")
			}

			instance = nullptr;

			application_info.sType = VkStructureType::VK_STRUCTURE_TYPE_APPLICATION_INFO;
			application_info.pNext = nullptr;
			application_info.pApplicationName = reinterpret_cast<const char*>(window_name);
			application_info.applicationVersion = 0;
			application_info.pEngineName = "xe";
			application_info.engineVersion = 0;
			application_info.apiVersion = cur_vulkan_version;

			instance_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instance_create_info.pNext = nullptr;
			instance_create_info.flags = 0;
			instance_create_info.pApplicationInfo = &application_info;
			instance_create_info.enabledExtensionCount = extension_number;
			instance_create_info.ppEnabledExtensionNames = extension;

#if defined(ENABLE_VULKAN_VALIDATION)
			if (is_support_enable_validation())
			{
				instance_create_info.enabledLayerCount = 1;
				instance_create_info.ppEnabledLayerNames = &(VULKAN_VALIDATION);
			}
#endif // ENABLE_VULKAN_VALIDATION IS END

			result = vkCreateInstance(&instance_create_info, nullptr, &instance);
			if (result != VK_SUCCESS)
			{
				XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI: Vulkan", "Vulkan instance init failed!");
				return false;
			}
			return true;
		}

		bool VulkanContext::find_physical_device() noexcept
		{
			uint32_t device_count = 0;
			dynamic_array<VkPhysicalDevice> devices;

			VkPhysicalDeviceProperties device_properties;
			VkPhysicalDeviceFeatures device_features;

			vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
			if (device_count == 0)
			{
				XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI: Vulkan", "failed to find GPUs with Vulkan support!");
				return false;
			}

			devices.resize(device_count);
			vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

			for (auto& vk_gpu : devices)
			{
				vkGetPhysicalDeviceProperties(vk_gpu, &device_properties);
				vkGetPhysicalDeviceFeatures(vk_gpu, &device_features);

				gpu_list.push_back(std::pair<String, VkPhysicalDevice>(device_properties.deviceName, vk_gpu));
			}

			if (gpu_list.size() == 0)
			{
				XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", "failed to find GPUs with Vulkan properties support!");
				return false;
			}
			return true;
		}

		bool VulkanContext::pick_up_physical_device(const String& gpu_name) noexcept
		{
			for (uint64_t i = 0; i < gpu_list.size(); i++)
			{
				if (gpu_name == gpu_list[i].first)
				{
					device_index = i;
					return true;
				}
			}
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI: Vulkan", "GPU pick up failed!");
			return false;
		}

		bool VulkanContext::create_logical_device(float* pqueue_priorities, int32_t queue_count) noexcept
		{
			VkDeviceCreateInfo device_create_info = {};

			dynamic_array<VkDeviceQueueCreateInfo> queue_create_info_list;
			VkDeviceQueueCreateInfo queue_create_info = {};

			VkPhysicalDeviceFeatures device_features = {};
			VkResult result;

			if (!get_device_queue_family())
			{
				XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeGraphicsAPI", "failed to get device queue!");
				return false;
			}

			if (!get_device_queue_family_support())
			{
				XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", "failed to find supporting queue!");
				return false;
			}

			XE_INFO_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", std::format("Use GPU [{0}]: {1}", device_index, gpu_list[device_index].first.c_str()).c_str());
			/*
			if (graphics_queue_index == present_queue_index)
			{
				queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queue_create_info.pQueuePriorities = pqueue_priorities;
				queue_create_info.queueCount = queue_count;
				queue_create_info.queueFamilyIndex = static_cast<uint32_t>(graphics_queue_index);
				queue_create_info_list.push_back(queue_create_info);
			}
			else
			*/
			queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_create_info.pQueuePriorities = pqueue_priorities;
			queue_create_info.queueCount = queue_count;
			queue_create_info.queueFamilyIndex = static_cast<uint32_t>(graphics_queue_index);
			queue_create_info_list.push_back(queue_create_info);
			queue_index_list.push_back(static_cast<uint32_t>(graphics_queue_index));
			queue_create_info.queueFamilyIndex = static_cast<uint32_t>(present_queue_index);
			queue_create_info_list.push_back(queue_create_info);
			queue_index_list.push_back(static_cast<uint32_t>(present_queue_index));

			device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			device_create_info.pQueueCreateInfos = queue_create_info_list.data();
			device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_info_list.size());
			device_create_info.pEnabledFeatures = &device_features;

			result = vkCreateDevice(gpu_list[device_index].second, &device_create_info, nullptr, &vk_device);
			if (result != VK_SUCCESS)
			{
				XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", "failed to create logical device!");
				return false;
			}

			return true;
		}

		bool VulkanContext::create_swap_chian(int32_t h, int32_t w)
		{
			bool state = vk_swap_chain_context.get_swap_chain_info(gpu_list[device_index].second, window_surface);
			if (!state)
				goto FAILED;
			vk_swap_chain_context.set_render_area(h, w);
			state = vk_swap_chain_context.init_swap_chain(vk_device, window_surface,
				2u, queue_index_list.data(), static_cast<uint32_t>(queue_index_list.size()));
			if (!state)
				goto FAILED;

		FAILED:
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeGraphicsAPI", "Get swap info failed!");
			return false;
		}

		void VulkanContext::release_surface() noexcept
		{
			if (window_surface)
			{
				vkDestroySurfaceKHR(instance, window_surface, nullptr);
			}
		}

		VulkanContext::~VulkanContext()
		{
			vkDestroyDevice(vk_device, nullptr);
			release_surface();
			if (instance)
			{
				vkDestroyInstance(instance, nullptr);
				instance = nullptr;
			}
		}

		bool VulkanContext::get_device_queue_family() noexcept
		{
			uint32_t queue_family_count = 0;
			queue_index_list.resize(4lu);
			vkGetPhysicalDeviceQueueFamilyProperties(gpu_list[device_index].second, &queue_family_count, nullptr);
			if(!queue_family_count)
			{
				XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", "No vulkan queue list");
			}
			queue_family_list = dynamic_array<VkQueueFamilyProperties>(queue_family_count);
			vkGetPhysicalDeviceQueueFamilyProperties(gpu_list[device_index].second, &queue_family_count, queue_family_list.data());
			return true;
		}

		bool VulkanContext::get_device_queue_family_support() noexcept
		{
			VkBool32 present_support = false;

			for (size_t i = 0; i < queue_family_list.size(); i++)
			{
				const auto& queue_family = queue_family_list[i];
				if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					graphics_queue_index = static_cast<uint32_t>(i);
					break;
				}
			}
			if (graphics_queue_index == -1)
			{
				XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", "failed to find a graphics queue family!");
				return false;
			}
			for (size_t i = 0; i < queue_family_list.size(); i++)
			{
				vkGetPhysicalDeviceSurfaceSupportKHR(gpu_list[device_index].second, static_cast<uint32_t>(i), window_surface, &present_support);
				if (present_support && i != graphics_queue_index)
				{
					present_queue_index = static_cast<uint32_t>(i);
					break;
				}
			}			
			if (present_queue_index == -1)
			{
				XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", "failed to find a present queue family!");
				return false;
			}
			return true;
		}
	} // namespace vulkan is end
} // namespace xe is end