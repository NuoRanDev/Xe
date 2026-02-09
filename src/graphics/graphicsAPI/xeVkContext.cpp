#include "graphicsAPI/xeVkContext.hpp"

#include "graphicsAPI/xeVkConfig.hpp"

#include "log/xeLogOutput.hpp"
#include "memory/xeAlloc.hpp"

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

			if (vkEnumerateInstanceVersion(&cur_vulkan_version) != VkResult::VK_SUCCESS)
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

		bool VulkanContext::find_physical_device(const dynamic_array<const char*>& input_want_extension_properties) noexcept
		{
			uint32_t device_count = 0;
			dynamic_array<VkPhysicalDevice> devices;

			VkPhysicalDeviceProperties device_properties;
			VkPhysicalDeviceFeatures device_features;

			std::copy(input_want_extension_properties.begin(), input_want_extension_properties.end(), std::back_inserter(want_extension_properties));

			vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
			if (device_count == 0)
			{
				XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI: Vulkan", "failed to find GPUs with Vulkan support!");
				return false;
			}

			devices.resize(device_count);
			vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

			for (auto& gpu : devices)
			{
				uint32_t extension_property_count = 0;
				dynamic_array<VkExtensionProperties> extension_properties;
				dynamic_array<char*> enable_device_extensions;
				uint32_t is_support_extension_property = 0;

				vkGetPhysicalDeviceProperties(gpu, &device_properties);
				vkGetPhysicalDeviceFeatures(gpu, &device_features);

				vkEnumerateDeviceExtensionProperties(gpu,nullptr, &extension_property_count, nullptr);
				extension_properties.resize(extension_property_count);
				vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extension_property_count, extension_properties.data());

				char* extension_property_str_space = xe_malloc<char>(extension_property_count * 256);
				memset(extension_property_str_space, 0, extension_property_count * 256);
				char* cur_extension_property_str = extension_property_str_space;
				
				for (const auto& extension : extension_properties)
				{
					if (is_support_extension_property != want_extension_properties.size())
					{
						for (const auto& want_extension_propertie : want_extension_properties)//"VK_KHR_swapchain"
						{
							if (std::strcmp(extension.extensionName, want_extension_propertie) == 0)
							{
								is_support_extension_property++;
								goto NEXT_EXTERSION;
							}
						}
					}
					NEXT_EXTERSION:
					memcpy(cur_extension_property_str, extension.extensionName, 256);

					enable_device_extensions.push_back(cur_extension_property_str);
					cur_extension_property_str += 256;
				}

				if(is_support_extension_property == want_extension_properties.size())
				{
					gpu_info_list.push_back(std::pair<String, dynamic_array<char*>>(device_properties.deviceName, enable_device_extensions));
					phy_dev_tree.push_back(gpu);
				}
			}

			device_index = phy_dev_tree.size() - 1;

			if (phy_dev_tree.size() == 0)
			{
				XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", "failed to find GPUs with wanted Vulkan properties support!");
				return false;
			}
			return true;
		}

		bool VulkanContext::pick_up_physical_device(const String& gpu_name) noexcept
		{
			for (uint64_t i = 0; i < gpu_info_list.size(); i++)
			{
				if (gpu_name == gpu_info_list[i].first)
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
			XE_CLR_WRITE_LINE("GPU INFO", XE_CLR_COLOR::GREEN);
			XE_CLR_WRITE_LINE(std::format("Use GPU [{0}]: {1}", device_index, gpu_info_list[device_index].first.c_str()).c_str());
			XE_CLR_WRITE_LINE("VULKAN EXTENSION", XE_CLR_COLOR::BLUE);
			for (size_t i = 0; i < gpu_info_list[device_index].second.size(); i++)
			{
				XE_CLR_WRITE_LINE(std::format("[{0}]:{1}", i, gpu_info_list[device_index].second[i]).c_str(), XE_CLR_COLOR::YELLOW);
			}
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
			//queue_index_list.resize(4lu);
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
			device_create_info.enabledExtensionCount = static_cast<uint32_t>(want_extension_properties.size());
			device_create_info.ppEnabledExtensionNames = want_extension_properties.data();

			result = vkCreateDevice(phy_dev_tree[device_index], &device_create_info, nullptr, &vk_device);
			if (result != VK_SUCCESS)
			{
				XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", "failed to create logical device!");
				return false;
			}

			return true;
		}

		bool VulkanContext::create_swap_chian(int32_t h, int32_t w)
		{
			bool state = vk_swap_chain_context.get_swap_chain_info(phy_dev_tree[device_index], window_surface);
			if (!state)
				goto INIT_SWAP_CHAIN_FAILED;
			vk_swap_chain_context.set_render_area(h, w);
			state = vk_swap_chain_context.init_swap_chain(vk_device, window_surface,
				1u, queue_index_list.data(), static_cast<uint32_t>(queue_index_list.size()));
			if (!state)
				goto INIT_SWAP_CHAIN_FAILED;
			return true;

		INIT_SWAP_CHAIN_FAILED:
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeGraphicsAPI", "Get swap info failed!");
			return false;
		}

		void VulkanContext::release_surface() noexcept
		{
			if (window_surface != nullptr)
			{
				vkDestroySurfaceKHR(instance, window_surface, nullptr);
			}
		}

		VulkanContext::~VulkanContext()
		{
			vk_swap_chain_context.release(vk_device);
			release_surface();
			if (vk_device != nullptr)
			{
				vkDestroyDevice(vk_device, nullptr);
			}
			if (instance != nullptr)
			{
				vkDestroyInstance(instance, nullptr);
				instance = nullptr;
			}

			for (auto& gpu_info : gpu_info_list)
			{
				xe_free(gpu_info.second[0]);
			}
		}

		bool VulkanContext::get_device_queue_family() noexcept
		{
			uint32_t queue_family_count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(phy_dev_tree[device_index], &queue_family_count, nullptr);
			if(!queue_family_count)
			{
				XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI : VULKAN", "No vulkan queue list");
			}
			queue_family_list = dynamic_array<VkQueueFamilyProperties>(queue_family_count);
			vkGetPhysicalDeviceQueueFamilyProperties(phy_dev_tree[device_index], &queue_family_count, queue_family_list.data());
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
				vkGetPhysicalDeviceSurfaceSupportKHR(phy_dev_tree[device_index], static_cast<uint32_t>(i), window_surface, &present_support);
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