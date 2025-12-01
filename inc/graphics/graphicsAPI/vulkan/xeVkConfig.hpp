#ifndef _INC_XE_VK_CONFIG_HPP_
#define _INC_XE_VK_CONFIG_HPP_

#include "vulkan/vulkan.h"

#ifdef _DEBUG
#define ENABLE_VULKAN_VALIDATION
#endif // _DEBUG IS END

namespace xe
{
	constexpr uint32_t vulkan_version = VK_MAKE_API_VERSION(0, 1, 0, 0);

#if defined(ENABLE_VULKAN_VALIDATION)
	constexpr const char* VULKAN_VALIDATION = "VK_LAYER_KHRONOS_validation";
	bool is_support_enable_validation() noexcept;
#endif // ENABLE_VULKAN_VALIDATION IS END

} // namespace xe is end

#endif // _INC_XE_VK_CONFIG_HPP_ IS EOF