#include "graphicsAPI/xeVkConfig.hpp"

#include "log/xeLogOutput.hpp"
#include "type/xeOrdinals.hpp"

namespace xe
{
#if defined(ENABLE_VULKAN_VALIDATION)
	bool is_support_enable_validation() noexcept
	{
		uint32_t instance_layer_present_count = 0;
		dynamic_array<VkLayerProperties> layer_props;

		// Get the layer count using a null pointer as the last parameter.
		vkEnumerateInstanceLayerProperties(&instance_layer_present_count, nullptr);
		if (instance_layer_present_count == 0)
			goto NOT_SUPPORT;

		// Enumerate layers with a valid pointer in the last parameter.
		layer_props = dynamic_array<VkLayerProperties>(instance_layer_present_count);
		vkEnumerateInstanceLayerProperties(&instance_layer_present_count, layer_props.data());

		// Make sure selected validation layers are available.
		for (auto& layer : layer_props) 
		{
			if (strcmp(layer.layerName, VULKAN_VALIDATION) == 0)
				return true;
		}

		NOT_SUPPORT:
		XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeGraphicsAPI: Vulkan", "Cna't support validation!")
		return false;
	}
#endif // ENABLE_VULKAN_VALIDATION IS END
} // namespace xe is end