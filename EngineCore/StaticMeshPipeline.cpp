#include "pch.h"
#include "StaticMeshPipeline.h"
#include "Device.h"
#include <array>

StaticMeshPipeline::StaticMeshPipeline(const std::string& a_name, std::weak_ptr<Device> a_device) : Pipeline(a_name, a_device)
{
	if (auto pDevice = m_device.lock())
	{
		VkPipelineCreateFlags flag;

		std::array<VkPipelineShaderStageCreateInfo, 2> stagesInfos = {
			initShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, VkShaderModule a_shaderModule),
			initShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, VkShaderModule a_shaderModule)
		}

		auto pipelineCreateInfo = createGraphicPipeline(, stagesInfos);
		VK_CHECK_EXCEPT(vkCreateGraphicsPipelines(pDevice->deviceContext().vkDevice, 1, &pipelineCreateInfo, nullptr, m_pipeline))
	}
}
