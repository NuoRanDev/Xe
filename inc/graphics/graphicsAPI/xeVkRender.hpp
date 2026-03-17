#include "vulkan/vulkan.h"

namespace xe
{
	class RenderProcess 
	{
	public:
		RenderProcess();

		void init_piple()
		{
			VkGraphicsPipelineCreateInfo gp_info = {};

			VkPipelineVertexInputStateCreateInfo vert = {};
			VkPipelineInputAssemblyStateCreateInfo in_asm = {};
			// vertex input
			gp_info.pVertexInputState = &vert;
			// vertex assembly
			in_asm.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			gp_info.pInputAssemblyState = &in_asm;
			// shader
			VkPipelineShaderStageCreateInfo shader_stage = {};

		}

		

	private:
		int64_t pid;

		VkPipeline pipline;
	};
} // namespace xe