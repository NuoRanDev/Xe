export module xe.Graphical.Vulkan;

import std;

import xe.Core.xeOrdinals;
import xe.Core.xeString;

import "vulkan/vulkan_core.h";


namespace xe
{
	export class xeVulkanContext
	{
	public:
		xeVulkanContext();

		bool SetupVulkanContext(const char* const* instance_extensions, xeUint32 instance_extensions_count, xeString context_name);

		VkInstance GetInstance() { return vulkan_instance; }

		VkSurfaceKHR* GetSurface() { return &g_surface; }

		std::pair<xeUint32, VkPhysicalDevice> GetPhysicalDevice() { return { physical_device_index, physical_device }; }

		void CleanupVulkanContext();

		~xeVulkanContext();

	private:
		bool CheckVulkanError(VkResult err);

		bool CreateInstance(const char* const* instance_extensions, xeUint32 instance_extensions_count, const char* instance_name);

		bool SetupVulkanSelectPhysicalDevice();

		bool SetQueueFamily();

		VkInstance					vulkan_instance;

		xeUint32	 				physical_device_index;
		VkPhysicalDevice			physical_device;

		VkDevice					virtual_device;

		VkSurfaceKHR				g_surface;
	};
}