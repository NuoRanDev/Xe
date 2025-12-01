#include "graphicsAPI/vulkan/xeVkContext.hpp"

using namespace xe;

int main()
{
	VulkanContext vk_context;
	xeString cur_gpu_name;

	puts("Initializing Vulkan...");
	if (!vk_context.init_vulkan_instance("MyVulkanApp")) {
		return -1; // Initialization failed
	}
	puts("Linking physical devices...");
	vk_context.link_physical_device();
	const auto& gpu_list = vk_context.get_physical_device_list();
	for (auto& gpu : gpu_list)
	{
		if (gpu.get_device_name().find_start(u8"nvidia", 7) != -1)
		{
			cur_gpu_name = gpu.get_device_name();
			return true;
		}
	}
	cur_gpu_name = gpu_list[0].get_device_name();
	puts("Creating logical device...");
	float nu = 0.1f;
	vk_context.create_logical_device(cur_gpu_name, &nu, 1);
	// Further code to use vk_context...
	return 0; // Success
}