#ifndef _XE_PHYSICAL_DEVICE_HPP_
#define _XE_PHYSICAL_DEVICE_HPP_

#include "string/xeString.hpp"

#include "type/xeOrdinals.hpp"

#include <any>

namespace xe
{
	enum class GPU_TYPE
	{
		CPU,
		DISCRETE_GPU,
		INTEGRATED_GPU,
		VIRTUAL_GPU
	};

	class Gpu final
	{
	public:

		bool is_discrete_gpu() const noexcept { return gpu_type == GPU_TYPE::DISCRETE_GPU; }

		GPU_TYPE get_gpu_type() const noexcept { return gpu_type; }

		const xeString& get_device_name() const noexcept { return device_name; }

		bool support_geometry_shader() const noexcept { return is_support_geometry_shader; }

		bool support_compute_shader() const noexcept { return is_support_compute_shader; }

	private:

		friend class VulkanGpuInstance;

		GPU_TYPE gpu_type;

		xeString device_name;

		std::any vk_physical_device;

		bool is_support_geometry_shader;

		bool is_support_compute_shader;
	};
} // namespace xe is end

#endif // _XE_PHYSICAL_DEVICE_HPP_ IS END