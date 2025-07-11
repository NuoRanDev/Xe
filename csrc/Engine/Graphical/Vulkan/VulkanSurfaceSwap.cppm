export module xe.Graphical.Vulkan.c;

import xe.Core.xeOrdinals;

namespace xe
{
	export class VulkanSurfaceCapabilities
	{
	public:
		VulkanSurfaceCapabilities() = default;
		bool IsSupportTextureFormat();
		~VulkanSurfaceCapabilities();
	};
}