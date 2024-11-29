#ifndef _INC_XE_VULKAN_INSTANCE_H_
#define _INC_XE_VULKAN_INSTANCE_H_

#include "xeGraphicalConfig.h"

namespace xe 
{
	class xeVulkanInstance
	{
	public:
		xeVulkanInstance();

		bool SetupVulkan(std::vector<const char*> instance_extensions);
		
		void CleanupVulkanWindow();
		
		~xeVulkanInstance();

	private:
		bool check_vk_result(VkResult err);
		VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();

		bool IsExtensionAvailable(const std::vector<VkExtensionProperties>& properties, const char* extension);

		VkAllocationCallbacks*		g_Allocator;
		VkInstance					g_Instance;
		VkPhysicalDevice			g_PhysicalDevice;
		VkDevice					g_Device;
		uint32_t					g_QueueFamily;
		VkQueue						g_Queue;
		VkDebugReportCallbackEXT	g_DebugReport;
		VkPipelineCache				g_PipelineCache;
		VkDescriptorPool			g_DescriptorPool;

		//static ImGui_ImplVulkanH_Window g_MainWindowData;
		uint32_t					g_MinImageCount = 2;
		bool						g_SwapChainRebuild = false;
	};
}

#endif // _INC_XE_VULKAN_INSTANCE_H_ IS EOF