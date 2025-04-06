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

		bool SetVulkanInstanceContext(const char* const* instance_extensions, xeUint32 instance_extensions_count, xeString context_name);

		bool SetVulkanLogicalDevice();

		VkInstance GetInstance() { return vulkan_instance; }

		VkSurfaceKHR* GetSurface() { return &window_surface; }

		std::pair<xeUint32, VkPhysicalDevice> GetPhysicalDevice() { return { physical_device_index, physical_device }; }

		void CleanupVulkanContext();

		~xeVulkanContext();

	private:
#if defined(_DEBUG)
		bool CheckValidationLayerSupport();
#endif // _DEBUG

		bool CheckVulkanError(VkResult err);

		bool CreateInstance(const char* const* instance_extensions, xeUint32 instance_extensions_count, const char* instance_name);

		bool SetupVulkanSelectPhysicalDevice();

		bool CheckQueueFamily();

		bool CreateLogicalDevice();

		VkInstance					vulkan_instance;

		xeUint32	 				physical_device_index;
		VkPhysicalDevice			physical_device;

		VkDevice					virtual_logical_device;

		VkQueue						graphics_queue;
		xeInt32						graphics_queue_family_index;
		VkQueue						present_queue;
		xeInt32						present_queue_family_index;

		VkSurfaceKHR				window_surface;

#if defined(_DEBUG)
		VkDebugUtilsMessengerEXT	debug_messenger;
		const std::vector<const char*> validation_layers = { "VK_LAYER_KHRONOS_validation" };
		bool						enable_validation_layers = false;
#endif // _DEBUG

	};
}