#include "pch.h"
#include "StaticMeshPipeline.h"
#include "Device.h"

StaticMeshPipeline::StaticMeshPipeline(const std::string& a_name, std::weak_ptr<Device> a_device, const std::vector<ShaderInfo>& a_shaders) : Pipeline(a_name, a_device)
{
	if (auto pDevice = m_device.lock())
	{
		std::vector<VkPipelineShaderStageCreateInfo> stagesInfos;
		for (const auto& [shaderType, shaderModule] : a_shaders)
			stagesInfos.emplace_back(initShaderStageCreateInfo(shaderType, shaderModule));
				
		auto pipelineCreateInfo = createGraphicPipeline(stagesInfos);
		VK_CHECK_EXCEPT(vkCreateGraphicsPipelines(pDevice->deviceContext().vkDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_pipeline))
	}
}
